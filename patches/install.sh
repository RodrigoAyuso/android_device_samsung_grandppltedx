#!/bin/sh

echo $1
rootdirectory="$PWD"
devicetree="device/samsung/grandppltedx"
# ---------------------------------

dirs="\
 bionic \
 build/make/core build/soong \
 frameworks/av frameworks/base frameworks/native
 hardware/interfaces \
 system/core system/sepolicy system/bt \
 external/wpa_supplicant_8 \
"

# red + nocolor
RED='\033[0;31m'
NC='\033[0m'

for dir in $dirs ; do
	cd $rootdirectory
	cd $dir
    echo -e "\n${RED}Applying ${NC}$dir ${RED}patches...${NC}\n"
	git apply -v $rootdirectory/$devicetree/patches/$dir/*.patch
done

# -----------------------------------
# MTK Audio Workaround (Bypass API 3.0)
# -----------------------------------
echo -e "\n${RED}Applying MTK Audio Workaround to Device.cpp...${NC}"

DEVICE_CPP="$rootdirectory/hardware/interfaces/audio/2.0/default/Device.cpp"

if [ -f "$DEVICE_CPP" ]; then
    if ! grep -q "AUDIO_DEVICE_API_VERSION_2_0" "$DEVICE_CPP"; then
        sed -i '/: mDevice(device) {/a \
    \n    // MTK Android 6.0 Workaround: Downgrade HAL version to 2.0 \
    // to bypass broken createAudioPatch ABI. \
    if (mDevice->common.version >= AUDIO_DEVICE_API_VERSION_3_0) { \
        ALOGI("Forcing audio HAL version to 2.0 for MTK workaround"); \
        mDevice->common.version = AUDIO_DEVICE_API_VERSION_2_0; \
    }' "$DEVICE_CPP"
        echo "Audio MTK patch applied successfully!"
    else
        echo "Audio MTK patch already applied, skipping."
    fi
else
    echo -e "Warning: Device.cpp not found at $DEVICE_CPP\nCould not apply MTK Audio patch."
fi
# -----------------------------------
echo -e "Done !\n"
cd $rootdirectory
