
#include "utils.hpp"
#include <filesystem>
#include <iostream>


void Utils::track_fixture(b2Fixture *fixture, const char *name)
{
    using json = nlohmann::json;

    json fixture_json;

    fixture_json["name"] = name;
    b2Shape* shape = fixture->GetShape();
    switch (shape->GetType()) {
        case b2Shape::e_circle: {
            b2CircleShape* circle = (b2CircleShape*)shape;
            fixture_json["type"] = "sphere";
            fixture_json["radius"] = circle->m_radius;
            break;
        }
        case b2Shape::e_polygon: {
            b2PolygonShape* cube = (b2PolygonShape*)shape;
            fixture_json["type"] = "cube";
            std::array<float, 3> hf = {2 * std::fabs(cube->m_vertices[0].x) , 2 * std::fabs(cube->m_vertices[0].y), 1};
            fixture_json["hf"] = hf;
            break;
        }
        default:
            break;
    }

    m_meta_json.push_back(fixture_json);

    m_fixtures[m_fixture_count++] = fixture;
}


static float sgn (float val) {
    return (0.0f < val) - (val < 0.0f);
}

void Utils::record_frame()
{

    using json = nlohmann::json;

    json frame_json;

    for (int i = 0; i < m_fixture_count; ++i) {
        b2Transform xf = m_fixtures[i]->GetBody()->GetTransform();
        auto p = xf.p;
        json status_json;
        status_json["position"] = {p.x, p.y, 0};

        float sin = xf.q.s;
        float cos = xf.q.c;

        float w = std::sqrt(sin + sin + 1 + 1);
        float x = 0;
        float y = 0;
        float z = sgn(2 * cos) * std::sqrt(1 - sin - sin + 1);

        status_json["quaternion"] = {w, x, y, z};
        frame_json[m_meta_json[i]["name"]] = status_json;
    }

    m_frame_json.push_back(frame_json);
}


void Utils::save_json_file()
{
    nlohmann::json json;
    json["meta"] = m_meta_json;
    json["frames"] = m_frame_json;

    std::ofstream file("scene.json");
    if (file.is_open()) {
        file << json.dump(4);
        file.close();
    } else {
        std::cerr << "Failed to open file";
    }
}


