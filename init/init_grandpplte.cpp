/*
 * Copyright (C) 2026 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_

#include <sys/_system_properties.h>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/strings.h>

#include <cstdio>
#include <string>
#include <unistd.h>
#include <vector>

#include "vendor_init.h"

#define SIMSLOT_FILE "/proc/simslot_count"

using android::base::GetProperty;
using android::base::ReadFileToString;
using android::base::StartsWith;

static const std::vector<std::string> kProductPropSources = {
  "",
  "odm.",
  "product.",
  "system.",
  "system_ext.",
  "vendor.",
};

static void property_override(const std::string& prop,
                              const std::string& value) {
  auto pi = const_cast<prop_info*>(
    __system_property_find(prop.c_str()));

  if (pi != nullptr) {
    __system_property_update(pi, value.c_str(), value.size());
  } else {
    __system_property_add(
      prop.c_str(), prop.size(),
                          value.c_str(), value.size());
  }
                              }

                              static void set_ro_product_prop(const std::string& prop,
                                                              const std::string& value) {
                                for (const auto& source : kProductPropSources) {
                                  property_override(
                                    "ro.product." + source + prop,
                                    value);
                                }
                                                              }

                                                              static int read_integer(const char* filename) {
                                                                int value;
                                                                FILE* file = fopen(filename, "r");

                                                                if (file == nullptr) {
                                                                  return -1;
                                                                }

                                                                if (fscanf(file, "%d", &value) != 1) {
                                                                  fclose(file);
                                                                  return -1;
                                                                }

                                                                fclose(file);
                                                                return value;
                                                              }

                                                              static std::string get_bootloader() {
                                                                std::string bootloader =
                                                                GetProperty("ro.bootloader", "");

                                                                if (bootloader.empty()) {
                                                                  bootloader =
                                                                  GetProperty("ro.boot.bootloader", "");
                                                                }

                                                                return bootloader;
                                                              }

                                                              static std::string detect_model_from_string(
                                                                const std::string& value) {
                                                                /*
                                                                 * G532MT must be checked before G532M because
                                                                 * "G532MT" also starts with "G532M".
                                                                 */
                                                                if (value.find("G532MT") != std::string::npos) {
                                                                  return "SM-G532MT";
                                                                }

                                                                if (value.find("G532M") != std::string::npos) {
                                                                  return "SM-G532M";
                                                                }

                                                                if (value.find("G532F") != std::string::npos) {
                                                                  return "SM-G532F";
                                                                }

                                                                if (value.find("G532G") != std::string::npos) {
                                                                  return "SM-G532G";
                                                                }

                                                                return "";
                                                                }

                                                                static std::string detect_model() {
                                                                  const std::string bootloader = get_bootloader();

                                                                  std::string model =
                                                                  detect_model_from_string(bootloader);

                                                                  if (!model.empty()) {
                                                                    LOG(INFO) << "grandpplte: detected "
                                                                    << model
                                                                    << " from bootloader "
                                                                    << bootloader;

                                                                    return model;
                                                                  }

                                                                  /*
                                                                   * Some Samsung/MediaTek boot chains expose the exact
                                                                   * product model in the "connie=" kernel command line.
                                                                   */
                                                                  std::string cmdline;

                                                                  if (ReadFileToString("/proc/cmdline", &cmdline)) {
                                                                    model = detect_model_from_string(cmdline);

                                                                    if (!model.empty()) {
                                                                      LOG(INFO) << "grandpplte: detected "
                                                                      << model
                                                                      << " from kernel cmdline";

                                                                      return model;
                                                                    }
                                                                  }

                                                                  return "";
                                                                }

                                                                static void set_device_model() {
                                                                  const std::string model = detect_model();

                                                                  if (model.empty()) {
                                                                    LOG(WARNING)
                                                                    << "grandpplte: unable to determine device model";
                                                                    return;
                                                                  }

                                                                  set_ro_product_prop("model", model);

                                                                  /*
                                                                   * Useful for init scripts and debugging without having
                                                                   * to re-parse the bootloader string.
                                                                   */
                                                                  property_override(
                                                                    "ro.vendor.grandpplte.model",
                                                                    model);

                                                                  LOG(INFO) << "grandpplte: using model "
                                                                  << model;
                                                                }

                                                                static void set_sim_properties() {
                                                                  int sim_count = -1;

                                                                  if (access(SIMSLOT_FILE, F_OK) == 0) {
                                                                    sim_count = read_integer(SIMSLOT_FILE);
                                                                  }

                                                                  if (sim_count == 1) {
                                                                    property_override(
                                                                      "ro.multisim.simslotcount",
                                                                      "1");

                                                                    property_override(
                                                                      "persist.radio.multisim.config",
                                                                      "none");

                                                                    LOG(INFO)
                                                                    << "grandpplte: detected single-SIM variant";
                                                                  } else if (sim_count >= 2) {
                                                                    property_override(
                                                                      "ro.multisim.simslotcount",
                                                                      "2");

                                                                    property_override(
                                                                      "persist.radio.multisim.config",
                                                                      "dsds");

                                                                    LOG(INFO)
                                                                    << "grandpplte: detected dual-SIM variant";
                                                                  } else {
                                                                    LOG(WARNING)
                                                                    << "grandpplte: unable to determine SIM slot count";
                                                                  }

                                                                  property_override(
                                                                    "ro.multisim.set_audio_params",
                                                                    "true");
                                                                }

                                                                void vendor_load_properties() {
                                                                  set_device_model();
                                                                  set_sim_properties();
                                                                }