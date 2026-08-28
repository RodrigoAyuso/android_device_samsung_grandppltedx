/*
 *  Copyright (c) 2018, The Lineage Project. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *
 * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *
 * Neither the name of The Linux Foundation nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 *  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 *  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 *  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 *  OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 *  IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <string>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#include "log.h"
#include "property_service.h"
#include "util.h"
#include "vendor_init.h"

#define SERIAL_NUMBER_FILE "/efs/FactoryApp/serial_no"
#define SIMSLOT_FILE "/proc/simslot_count"

using android::base::GetProperty;
using android::init::property_set;

/*
 * Samsung Galaxy J2 Prime / Grand Prime Plus variants.
 *
 * Important:
 * G532MT must be checked before G532M because "G532MT"
 * also contains the substring "G532M".
 */
struct device_variant {
	const char* bootloader;
	const char* model;
	const char* product_name;
	const char* product_device;
};

static const device_variant variants[] = {
	{
		"G532MT",
		"SM-G532MT",
		"grandppltedtvvj",
		"grandppltedtv"
	},
	{
		"G532M",
		"SM-G532M",
		"grandpplteub",
		"grandpplte"
	},
	{
		"G532G",
		"SM-G532G",
		"grandppltedx",
		"grandpplte"
	},
	{
		"G532F",
		"SM-G532F",
		"grandpplteser",
		"grandpplte"
	}
};

/*
 * Read an integer from a proc/sysfs file.
 */
static int read_integer(const char* filename) {
	int retval = -1;
	FILE* file = fopen(filename, "r");

	if (file == nullptr) {
		return -1;
	}

	if (fscanf(file, "%d", &retval) != 1) {
		retval = -1;
	}

	fclose(file);

	return retval;
}

/*
 * Override a read-only Android property.
 *
 * This is intentionally kept compatible with the implementation
 * already used by this LineageOS 15.1 device tree.
 */
static void property_override(const char* prop, const char* value) {
	prop_info* pi;

	pi = const_cast<prop_info*>(__system_property_find(prop));

	if (pi != nullptr) {
		__system_property_update(pi, value, strlen(value));
	} else {
		__system_property_add(
			prop,
			strlen(prop),
							  value,
						strlen(value)
		);
	}
}

/*
 * Set both regular and vendor product properties.
 *
 * Existing tree already uses:
 *
 *   ro.product.*
 *   ro.vendor.product.*
 */
static void property_override_dual(
	const char* system_prop,
	const char* vendor_prop,
	const char* value) {

	property_override(system_prop, value);
	property_override(vendor_prop, value);
	}

	/*
	 * Configure dual-SIM properties.
	 */
	static void init_dual() {
		property_set("ro.multisim.set_audio_params", "true");
		property_set("ro.multisim.simslotcount", "2");
		property_set("persist.radio.multisim.config", "dsds");
	}

	/*
	 * Configure single-SIM properties.
	 */
	static void init_single() {
		property_set("ro.multisim.set_audio_params", "true");
		property_set("ro.multisim.simslotcount", "1");
		property_set("persist.radio.multisim.config", "none");
	}

	/*
	 * Find the actual J2 Prime hardware variant from the Samsung
	 * bootloader string.
	 */
	static const device_variant* get_device_variant(
		const std::string& bootloader) {

		for (const auto& variant : variants) {
			if (bootloader.find(variant.bootloader) != std::string::npos) {
				return &variant;
			}
		}

		return nullptr;
		}

		/*
		 * Set all model-dependent Android properties.
		 */
		static void set_device_properties(
			const device_variant& variant,
			const std::string& bootloader,
			bool dual_sim) {

			property_override_dual(
				"ro.product.name",
				"ro.vendor.product.name",
				variant.product_name
			);

			property_override_dual(
				"ro.product.device",
				"ro.vendor.product.device",
				variant.product_device
			);

			property_override_dual(
				"ro.product.model",
				"ro.vendor.product.model",
				variant.model
			);

			property_override(
				"ro.build.product",
				variant.product_device
			);

			/*
			 * ro.bootloader contains the Samsung AP/PDA revision.
			 *
			 * Example:
			 * G532MTVJU1AQA2
			 */
			std::string fingerprint =
			"samsung/" +
			std::string(variant.product_name) + "/" +
			std::string(variant.product_device) +
			":6.0.1/MMB29T/" +
			bootloader +
			":user/release-keys";

			std::string description =
			std::string(variant.product_name) +
			"-user 6.0.1 MMB29T " +
			bootloader +
			" release-keys";

			property_override(
				"ro.build.fingerprint",
				fingerprint.c_str()
			);

			property_override(
				"ro.vendor.build.fingerprint",
				fingerprint.c_str()
			);

			property_override(
				"ro.build.description",
				description.c_str()
			);

			property_override(
				"ro.build.PDA",
				bootloader.c_str()
			);

			property_override(
				"ro.build.hidden_ver",
				bootloader.c_str()
			);

			std::string lineage_model = variant.model;

			if (dual_sim) {
				lineage_model += "/DS";
			}

			property_override(
				"ro.lineage.device.model",
				lineage_model.c_str()
			);

			property_override(
				"ro.lineage.device.sim_type",
				dual_sim ? "dual" : "single"
			);
			}

			void vendor_load_properties() {
				const std::string bootloader =
				GetProperty("ro.bootloader", "");

				const std::string platform =
				GetProperty("ro.board.platform", "");

				/*
				 * Make sure this libinit is running on the expected SoC.
				 */
				if (platform != ANDROID_TARGET) {
					return;
				}

				/*
				 * Detect number of SIM slots.
				 *
				 * Preserve the old tree's behavior:
				 * 1 = single
				 * anything else = dual
				 *
				 * This means that if /proc/simslot_count cannot be read,
				 * we still fall back to dual-SIM instead of disabling the
				 * second RIL unexpectedly.
				 */
				int sim_count = -1;

				if (access(SIMSLOT_FILE, F_OK) == 0) {
					sim_count = read_integer(SIMSLOT_FILE);
				}

				const bool dual_sim = (sim_count != 1);

				if (dual_sim) {
					init_dual();
				} else {
					init_single();
				}

				/*
				 * Detect G532F/G532G/G532M/G532MT.
				 */
				const device_variant* variant =
				get_device_variant(bootloader);

				if (variant != nullptr) {
					set_device_properties(*variant, bootloader, dual_sim);
				}
			}