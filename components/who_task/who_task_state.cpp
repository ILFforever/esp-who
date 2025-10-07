#include "who_task_state.hpp"
#include <freertos/FreeRTOS.h>

namespace who {
namespace task {
WhoTaskState::WhoTaskState(int interval) : WhoTask("TaskState"), m_interval(interval)
{
    m_task_state = {"Running", "Ready", "Blocked", "Suspended", "Deleted", "Invalid"};
}

void WhoTaskState::task()
{
    const TickType_t interval = pdMS_TO_TICKS(1000 * m_interval);
    TickType_t last_wake_time = xTaskGetTickCount();
    while (true) {
        vTaskDelayUntil(&last_wake_time, interval);
        EventBits_t event_bits = xEventGroupWaitBits(m_event_group, TASK_PAUSE | TASK_STOP, pdTRUE, pdFALSE, 0);
        if (event_bits & TASK_STOP) {
            break;
        } else if (event_bits & TASK_PAUSE) {
            xEventGroupSetBits(m_event_group, TASK_PAUSED);
            EventBits_t pause_event_bits =
                xEventGroupWaitBits(m_event_group, TASK_RESUME | TASK_STOP, pdTRUE, pdFALSE, portMAX_DELAY);
            if (pause_event_bits & TASK_STOP) {
                break;
            } else {
                last_wake_time = xTaskGetTickCount();
                continue;
            }
        }
    }
    xEventGroupSetBits(m_event_group, TASK_STOPPED);
    vTaskDelete(NULL);
}

bool WhoTaskState::stop_async()
{
    if (WhoTask::stop_async()) {
        xTaskAbortDelay(m_task_handle);
        return true;
    }
    return false;
}

bool WhoTaskState::pause_async()
{
    if (WhoTask::pause_async()) {
        xTaskAbortDelay(m_task_handle);
        return true;
    }
    return false;
}

} // namespace task
} // namespace who
