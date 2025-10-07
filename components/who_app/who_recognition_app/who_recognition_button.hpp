#pragma once
#include "who_recognition.hpp"
#include "bsp/esp-bsp.h"

namespace who {
namespace button {
class WhoRecognitionButton {
public:
    typedef struct {
        task::WhoTask *task;
        EventBits_t event;
    } btn_user_data_t;
    WhoRecognitionButton(task::WhoTask *task);
    virtual ~WhoRecognitionButton();

protected:
    btn_user_data_t *m_btn_user_data;
};

#if BSP_CAPS_BUTTONS
class WhoRecognitionButtonPhysical : public WhoRecognitionButton {
public:
    WhoRecognitionButtonPhysical(recognition::WhoRecognitionCore *recognition);
    ~WhoRecognitionButtonPhysical();

private:
    static void btn_event_handler(void *button_handle, void *usr_data);
    button_handle_t *m_btns;
};
#endif

// Only define LVGL button class if BSP has display support
#if CONFIG_BSP_DISPLAY_ENABLED || defined(CONFIG_LCD_ENABLE)
#include "lvgl.h"

class WhoRecognitionButtonLVGL : public WhoRecognitionButton {
public:
    WhoRecognitionButtonLVGL(recognition::WhoRecognitionCore *recognition);
    ~WhoRecognitionButtonLVGL();

private:
    static void btn_event_handler(lv_event_t *e);
    lv_obj_t *m_btn_recognize;
    lv_obj_t *m_btn_enroll;
    lv_obj_t *m_btn_delete;
};
#endif

enum class recognition_button_type_t {
    PHYSICAL,
    LVGL,
};

WhoRecognitionButton *get_recognition_button(recognition_button_type_t btn_type,
                                             recognition::WhoRecognitionCore *recognition);
} // namespace button
} // namespace who