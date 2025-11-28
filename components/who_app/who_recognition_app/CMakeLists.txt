set(src_dirs        .
                    ../who_app_common)
                    # Excluding who_detect_result_handle and who_text_result_handle due to API incompatibility with esp-dl

set(include_dirs    .
                    ../who_app_common)

set(requires who_recognition who_frame_lcd_disp)

# Exclude LCD app files that depend on who_detect_result_handle
set(exclude_srcs    "who_recognition_app_lcd.cpp")

idf_component_register(SRC_DIRS ${src_dirs}
                       INCLUDE_DIRS ${include_dirs}
                       REQUIRES ${requires}
                       EXCLUDE_SRCS ${exclude_srcs})