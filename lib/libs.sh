#! /bin/bash

cd assimp
#git reset --hard c35200e38ea8f058812b83de2ef32c6093b0ece2
git reset --hard v5.4.3
cd ..

cd glad
git reset --hard origin/c
cd ..

cd glfw
# origin/ci
git reset --hard b35641f4a3c62aa86a0b3c983d163bc0fe36026d
cd ..

cd imgui
git reset --hard 1d88609043fd7a25df7f5f4acd7aa3cc4aaf8033
cd ..

cd stb
git reset --hard origin/stb_image
cd ..
