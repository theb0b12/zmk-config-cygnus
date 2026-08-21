#include <zephyr/kernel.h>
#include <zmk/event_manager.h>
#include <raw_hid.h>
#include <string.h>

// This function runs whenever your computer sends a packet over Raw HID to the keyboard
static int raw_hid_received_event_listener(const zmk_event_t *eh) {
    struct raw_hid_received_event *event = as_raw_hid_received_event(eh);
    if (event) {
        // event->data contains the bytes sent from Python
        // event->length contains how many bytes were received
        
        // Example: If Python sent a command byte (e.g., 0xAF), let's reply back!
        if (event->data[0] == 0xAF) {
            uint8_t response[32];
            memset(response, 0, sizeof(response));
            
            response[0] = 0x01; // Report ID
            response[1] = 0x01; // Current layer (hardcoded for test)
            response[2] = 0x64; // Battery level (100%)
            
            // Send data back to the computer
            raw_hid_send(response, sizeof(response));
        }
    }
    return ZMK_EV_EVENT_BUBBLE;
}

ZMK_LISTENER(process_raw_hid_event, raw_hid_received_event_listener);
ZMK_SUBSCRIPTION(process_raw_hid_event, raw_hid_received_event);