#pragma once
#include "human_face_recognition.hpp"
#include "who_detect.hpp"

namespace who {
namespace recognition {
class WhoRecognitionCore : public task::WhoTask {
public:
    static inline constexpr EventBits_t RECOGNIZE = TASK_EVENT_BIT_LAST;
    static inline constexpr EventBits_t ENROLL = TASK_EVENT_BIT_LAST << 1;
    static inline constexpr EventBits_t DELETE = TASK_EVENT_BIT_LAST << 2;

    WhoRecognitionCore(const std::string &name, detect::WhoDetect *detect);
    ~WhoRecognitionCore();
    void set_recognizer(HumanFaceRecognizer *recognizer);
    void set_recognition_result_cb(const std::function<void(const std::string &)> &result_cb);
    void set_detect_result_cb(const std::function<void(const detect::WhoDetect::result_t &)> &result_cb);
    void set_cleanup_func(const std::function<void()> &cleanup_func);
    bool run(const configSTACK_DEPTH_TYPE uxStackDepth, UBaseType_t uxPriority, const BaseType_t xCoreID) override;

private:
    void task() override;
    void cleanup() override;
    detect::WhoDetect *m_detect;
    HumanFaceRecognizer *m_recognizer;
    std::function<void(const detect::WhoDetect::result_t &)> m_detect_result_cb;
    std::function<void(const std::string &)> m_recognition_result_cb;
    std::function<void()> m_cleanup;
};

class WhoRecognition : public task::WhoTaskGroup {
public:
    WhoRecognition(frame_cap::WhoFrameCapNode *frame_cap_node);
    ~WhoRecognition();
    void set_detect_model(dl::detect::Detect *model);
    void set_recognizer(HumanFaceRecognizer *recognizer);
    detect::WhoDetect *get_detect_task();
    WhoRecognitionCore *get_recognition_task();

    // Power management methods
    void shutdown();  // Stop tasks without deleting objects
    bool restart();   // Restart stopped tasks
    void mark_running();  // Mark tasks as running (call after initial run())

private:
    detect::WhoDetect *m_detect;
    WhoRecognitionCore *m_recognition;

    // Store run parameters for restart
    configSTACK_DEPTH_TYPE m_detect_stack = 3584;
    UBaseType_t m_detect_priority = 2;
    BaseType_t m_detect_core = 1;
    configSTACK_DEPTH_TYPE m_recognition_stack = 3584;
    UBaseType_t m_recognition_priority = 2;
    BaseType_t m_recognition_core = 1;
    bool m_is_running = false;
};
} // namespace recognition
} // namespace who
