find_package(Rock)
include(RockRuby)

IF(RUBY_EXTENSIONS_AVAILABLE)
    execute_process(RESULT_VARIABLE SWIG_RESULT WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/bindings COMMAND swig -c++ -ruby ${PROJECT_NAME}.i)
    if(SWIG_RESULT EQUAL 0)
        ROCK_RUBY_EXTENSION(r${PROJECT_NAME} ${PROJECT_NAME}_wrap.cxx)
    elseif(SWIG_RESULT EQUAL 1)
        message(WARNING "Cannot Build Ruby Binding because SWIG returned an error")
    else(SWIG_RESULT EQUAL 0)
        message(WARNING "Cannot Build Ruby Binding because SWIG is not installed") 
    endif(SWIG_RESULT EQUAL 0)
ENDIF(RUBY_EXTENSIONS_AVAILABLE)

