/*
 * Legacy libunwindstack ABI compatibility for old MTK blobs.
 *
 * Older libladder.so expects the out-of-line Maps destructor that
 * disappeared after Maps::~Maps() became inline/defaulted.
 */

extern "C" {

    __attribute__((visibility("default")))
    void maps_destructor_base(void* /* self */)
    __asm__("_ZN11unwindstack4MapsD2Ev");

    void maps_destructor_base(void* /* self */) {
        // Compatibility stub.
    }

    __attribute__((visibility("default")))
    void maps_destructor_complete(void* /* self */)
    __asm__("_ZN11unwindstack4MapsD1Ev");

    void maps_destructor_complete(void* /* self */) {
        // Compatibility stub.
    }

}