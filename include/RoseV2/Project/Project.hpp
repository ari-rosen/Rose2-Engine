#pragma once

namespace RoseV2::Project {

struct Project {
    std::string Name = "Untitled";
    std::filesystem::path StartScene;
    std::filesystem::path AssetDirectory;
};

}
