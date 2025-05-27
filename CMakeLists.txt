cmake_minimum_required(VERSION 3.10)
project(PDS_Analysis_BSM)

find_package(ROOT REQUIRED COMPONENTS Core Tree RIO Hist)
include(${ROOT_USE_FILE})

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

# Dicionário ROOT para my_data
set(DICT_HEADERS ${PROJECT_SOURCE_DIR}/include/my_data.hh)
set(DICT_LINKDEF ${PROJECT_SOURCE_DIR}/include/LinkDef.h)

# Gera os arquivos my_dataDict.cxx e my_dataDict_rdict.pcm
ROOT_GENERATE_DICTIONARY(my_dataDict ${DICT_HEADERS} LINKDEF ${DICT_LINKDEF})

# Adiciona os arquivos do dicionário como parte das sources
add_library(my_dataDictLib my_dataDict.cxx)

# Definindo o diretório do executável
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Criação do executável
add_executable(run main.cpp ${sources})

# Criação do executável_2
#add_executable(run2 main_2.cpp ${sources})

# Criação do executável_3
#add_executable(run3 main_3.cpp ${sources})

# Criação do executável_4
add_executable(run4 gera_hist.cpp ${sources})

# Adicionando as flags manualmente
# Passando as opções de compilação corretamente para o CMake
string(REPLACE " " ";" ROOT_CFLAGS_LIST ${ROOT_CFLAGS})

target_include_directories(my_dataDictLib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(run PRIVATE my_dataDictLib)

# Incluindo os diretórios de header e as bibliotecas corretamente
target_include_directories(run PRIVATE ${ROOT_CFLAGS_LIST} ${PROJECT_SOURCE_DIR}/include)
target_compile_options(run PRIVATE ${ROOT_CFLAGS_LIST})

# Linkando as bibliotecas do ROOT
target_link_libraries(run PRIVATE ${ROOT_LIBS})

#--------------------------------------------------------------
target_include_directories(my_dataDictLib PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(run4 PRIVATE my_dataDictLib)

# Incluindo os diretórios de header e as bibliotecas corretamente
target_include_directories(run4 PRIVATE ${ROOT_CFLAGS_LIST} ${PROJECT_SOURCE_DIR}/include)
target_compile_options(run4 PRIVATE ${ROOT_CFLAGS_LIST})

# Linkando as bibliotecas do ROOT
target_link_libraries(run4 PRIVATE ${ROOT_LIBS})




