#pragma once
#include "ofxImGui.h"
#include "ofParameter.h"

class ofxImGuiHelpers
{
public:
    static void ParameterGroup(ofParameterGroup parameters)
    {
        for (std::size_t i = 0; i < parameters.size(); i++)
        {
            auto type = parameters.getType(i);
            // ofLogNotice() << type;
            if (type == typeid(ofParameter<int32_t>).name())
            {
                auto p = parameters.getInt(i);
                // ofLogNotice() << "int32 " << p.getName();
                int value = p.get();
                if (p.getMax() - p.getMin() <= 10)
                {
                    if (ImGui::InputInt(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                    {
                        p = value;
                    }
                }
                else
                {
                    if (ImGui::SliderInt(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                    {
                        p = value;
                    }
                }
            }
            else if (type == typeid(ofParameter<uint32_t>).name())
            {
                auto p = parameters.get<uint32_t>(i);
                int value = p.get();
                if (p.getMax() - p.getMin() <= 10)
                {
                    if (ImGui::InputInt(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                    {
                        p = value;
                    }
                }
                else
                {
                    if (ImGui::SliderInt(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                    {
                        p = value;
                    }
                }
            }
            else if (type == typeid(ofParameter<int64_t>).name())
            {
                auto p = parameters.get<int64_t>(i);
                // ofLogNotice() << "int64 " << p.getName();
                int value = p.get();
                if (ImGui::InputInt(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                {
                    p = value;
                }
            }
            else if (type == typeid(ofParameter<uint64_t>).name())
            {
                auto p = parameters.get<uint64_t>(i);
                int value = p.get();
                if (ImGui::InputInt(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                {
                    p = value;
                }
            }
            else if (type == typeid(ofParameter<int8_t>).name())
            {
                auto p = parameters.get<int8_t>(i);
                int value = p.get();
                if (ImGui::InputInt(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                {
                    p = value;
                }
            }
            else if (type == typeid(ofParameter<uint8_t>).name())
            {
                auto p = parameters.get<uint8_t>(i);
                int value = p.get();
                if (ImGui::InputInt(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                {
                    p = value;
                }
            }
            else if (type == typeid(ofParameter<int16_t>).name())
            {
                auto p = parameters.get<int16_t>(i);
                int value = p.get();
                if (ImGui::InputInt(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                {
                    p = value;
                }
            }
            else if (type == typeid(ofParameter<uint16_t>).name())
            {
                auto p = parameters.get<uint16_t>(i);
                int value = p.get();
                if (ImGui::InputInt(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                {
                    p = value;
                }
            }
            else if (type == typeid(ofParameter<size_t>).name())
            {
                auto p = parameters.get<size_t>(i);
                // add(p);
            }
            else if (type == typeid(ofParameter<float>).name())
            {
                auto p = parameters.getFloat(i);
                auto value = p.get();
                if (ImGui::SliderFloat(p.getName().c_str(), &value, p.getMin(), p.getMax()))
                {
                    p = value;
                }
            }
            else if (type == typeid(ofParameter<double>).name())
            {
                auto p = parameters.get<double>(i);
            }
            else if (type == typeid(ofParameter<bool>).name())
            {
                auto p = parameters.getBool(i);
                auto value = p.get();
                if (ImGui::Checkbox(p.getName().c_str(), &value))
                {
                    p = !p;
                }
            }
            else if (type == typeid(ofParameter<void>).name())
            {
                auto p = parameters.getVoid(i);
                if (ImGui::Button((p.getName() + "##clip_" + p.getName()).c_str()))
                {
                    p.trigger();
                }
            }
            else if (type == typeid(ofParameter<ofVec2f>).name())
            {
                auto p = parameters.get<ofVec2f>(i);
                // add(p);
            }
            else if (type == typeid(ofParameter<ofVec3f>).name())
            {
                auto p = parameters.get<ofVec3f>(i);
                // add(p);
            }
            else if (type == typeid(ofParameter<ofVec4f>).name())
            {
                auto p = parameters.get<ofVec4f>(i);
                // add(p);
            }
            else if (type == typeid(ofParameter<glm::vec2>).name())
            {
                auto p = parameters.get<glm::vec2>(i);
                // add(p);
            }
            else if (type == typeid(ofParameter<glm::vec3>).name())
            {
                auto p = parameters.get<glm::vec3>(i);
                // add(p);
            }
            else if (type == typeid(ofParameter<glm::vec4>).name())
            {
                auto p = parameters.get<glm::vec4>(i);
                // add(p);
            }
            else if (type == typeid(ofParameter<ofColor>).name())
            {
                auto p = parameters.getColor(i);
                // add(p);
            }
            else if (type == typeid(ofParameter<ofShortColor>).name())
            {
                auto p = parameters.getShortColor(i);
                // add(p);
            }
            else if (type == typeid(ofParameter<ofFloatColor>).name())
            {
                auto p = parameters.getFloatColor(i);
                // add(p);
            }
            else if (type == typeid(ofParameter<ofRectangle>).name())
            {
                auto p = parameters.getRectangle(i);
                // add(p);
            }
            else if (parameters[i].valueType() == typeid(string).name())
            {
                if (parameters[i].isReadOnly())
                {
                    auto p = parameters.get(i).castReadOnly<std::string, void>();
                    // add(p);
                }
                else
                {
                    auto p = parameters.getString(i);
                    // add(p);
                }
            }
            else if (type == typeid(ofParameterGroup).name())
            {
                auto p = parameters.getGroup(i);
                if (ImGui::CollapsingHeader(p.getName().c_str()))
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (float)16);
                    ImGui::BeginGroup();
                    ParameterGroup(p);
                    ImGui::EndGroup();
                }

                // add(createGuiGroup(p));
            }
            else
            {
                ofLogWarning() << "ofxBaseGroup; no control for parameter of type " << type;
            }
            // ofLogNotice() << "adding type " << type;
        }
    }
};