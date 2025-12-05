# LglEngine
Light Graphics Library Engine
- Package management is not yet implemented
- Thus building requires the manual download of two zip files
- The cloud link: https://drive.google.com/drive/folders/1OFFZljUOw9g_wUwm8N1OqbQ_mMzPy7dP?usp=sharing
- unzip assets.zip at **'LglEngine/res'** so it fits **'LglEngine/res/assets/*'**
- unzip alldeps.zip at **'LglEngine/dep'** so it fits **'LglEngine/dep/alldeps/*'**
- run **cmake --preset x64-Debug-VS2022** from repo root to generate build system
- run **cmake --build out/build --config Debug --target Demo** from repo root to build
- start **Demo.exe**, has to be started from bin folder (since it needs .dll .ini etc.)
