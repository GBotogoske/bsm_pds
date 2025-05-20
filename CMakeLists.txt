cmake_minimum_required(VERSION 3.10)
project(PDS_Analysis_BSM)

set(CMAKE_CXX_STANDARD 17)

# ROOT CFLAGS e LIBS
execute_process(
  COMMAND root-config --cflags
  OUTPUT_VARIABLE ROOT_CFLAGS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
  COMMAND root-config --libs
  OUTPUT_VARIABLE ROOT_LIBS
  OUTPUT_STRIP_TRAILING_WHITESPACE
)

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

# Definindo o diretório do executável
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Criação do executável
add_executable(run main.cpp ${sources})

# Criação do executável_2
add_executable(run2 main_2.cpp ${sources})

# Criação do executável_3
add_executable(run3 main_3.cpp ${sources})

# Adicionando as flags manualmente
# Passando as opções de compilação corretamente para o CMake
string(REPLACE " " ";" ROOT_CFLAGS_LIST ${ROOT_CFLAGS})

# Incluindo os diretórios de header e as bibliotecas corretamente
target_include_directories(run PRIVATE ${ROOT_CFLAGS_LIST} ${PROJECT_SOURCE_DIR}/include)
target_compile_options(run PRIVATE ${ROOT_CFLAGS_LIST})

# Linkando as bibliotecas do ROOT
target_link_libraries(run PRIVATE ${ROOT_LIBS})

# Incluindo os diretórios de header e as bibliotecas corretamente
target_include_directories(run2 PRIVATE ${ROOT_CFLAGS_LIST} ${PROJECT_SOURCE_DIR}/include)
target_compile_options(run2 PRIVATE ${ROOT_CFLAGS_LIST})

# Linkando as bibliotecas do ROOT
target_link_libraries(run2 PRIVATE ${ROOT_LIBS})

# Incluindo os diretórios de header e as bibliotecas corretamente
target_include_directories(run3 PRIVATE ${ROOT_CFLAGS_LIST} ${PROJECT_SOURCE_DIR}/include)
target_compile_options(run3 PRIVATE ${ROOT_CFLAGS_LIST})

# Linkando as bibliotecas do ROOT
target_link_libraries(run3 PRIVATE ${ROOT_LIBS})




