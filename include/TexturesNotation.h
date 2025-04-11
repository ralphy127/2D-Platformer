#pragma once;

#include <string>
#include <unordered_map>

enum class TextureId {
    Canopy_01 = 101,
    Canopy_02,
    Canopy_03,
    Canopy_04,
    
    Chimney_01,
    Chimney_02,
    Chimney_03,
    Chimney_04,
    
    Decor_Roof_01,
    Decor_Roof_02,
    
    Decor_Window_01,
    Decor_Window_02,
    Decor_Window_03,
    Decor_Window_04,
    Decor_Window_05,
    Decor_Window_06,
    Decor_Window_07,
    Decor_Window_08,

    Door_01,
    Door_02,
    Door_03,
    Door_04,
    
    Ladder_01,
    Ladder_02,
    
    Pillar_01,
    Pillar_02,
    Pillar_03,
    Pillar_04,
    Pillar_05,
    Pillar_06,
    Pillar_07,
    
    Roof_A_01,
    Roof_A_02,
    Roof_A_03,
    Roof_A_04,
    Roof_A_05,
    
    Roof_B_01,
    Roof_B_02,
    Roof_B_03,
    Roof_B_04,
    Roof_B_05,
    
    Stone_Window_01,
    Stone_Window_02,
    Stone_Window_03,
    
    Wall_A_01,
    Wall_A_02,
    Wall_A_03,
    
    Wall_B_01,
    Wall_B_02,
    Wall_B_03,
    
    Wall_C_01,
    Wall_C_02,
    Wall_C_03,
    
    Wide_Door_01,
    Wide_Door_02,
    Wide_Door_03,
    Wide_Door_04,
    
    Window_01,
    Window_02,
    Window_03
};

namespace TexturePaths {
    const std::unordered_map<TextureId, std::string> PATHS = {
        {TextureId::Canopy_01, "textures/Village/Building/Canopy_01.png"},
        // reszta
        {TextureId::Wall_A_01, "textures/Village/Building/Wall_A_01.png"},
        {TextureId::Wall_A_02, "textures/Village/Building/Wall_A_02.png"},
        {TextureId::Wall_A_02, "textures/Village/Building/Wall_A_02.png"},
    };

    inline const std::string& get(TextureId id) {
        static const std::string EMPTY = "";
        auto it = PATHS.find(id);
        return it != PATHS.end() ? it->second : EMPTY;
    }
}