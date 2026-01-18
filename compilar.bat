@REM Para GCC,  precisamos setar a flag na geracao do projeto
@REM Para MSVC, precisamos passar a flag "config" na construcao, pois
@REM ele ignora a variavel passada na geracao (eh um generator multiconfig)
@REM https://stackoverflow.com/
@REM /questions/24460486/cmake-build-type-is-not-being-used-in-cmakelists-txt
cmake -S . -B out -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_UNIT_TESTS=OFF
cmake --build out --config RelWithDebInfo --target install
