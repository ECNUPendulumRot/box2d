
#ifndef BOX3D_UTILS_HPP
#define BOX3D_UTILS_HPP


#include "box2d/box2d.h"
#include "json.hpp"
#include <fstream>


class Utils {

    b2Fixture* m_fixtures[1024];

    b2Body* m_bodies[1024];
    std::string m_body_names[1024];
    int32 m_fixture_count = 0;
    int32 m_body_count = 0;

    nlohmann::json m_meta_json;
    nlohmann::json m_frame_json;

    bool file_is_open = false;
    bool json_saved = false;
    std::ofstream csv_file;

public:

    void open_file();

    void save_body_info();

    inline void close_file() {
        if (file_is_open) {
            csv_file.close();
        }
    }

    void track_fixture(b2Fixture *fixture, const char *name);

    void track_body(b2Body* body, const char* name);

    void record_frame();

    void save_json_file();

    ~Utils() {
        close_file();
        file_is_open = false;
    }

};


#endif //BOX3D_UTILS_HPP
