project(ConicBundle)
cmake_minimum_required(VERSION 3.1.2)

# compiler options
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


FUNCTION(PREPEND var prefix)
    SET(listVar "")
    FOREACH(f ${ARGN})
        LIST(APPEND listVar "${prefix}/${f}")
    ENDFOREACH(f)
    SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

SET(CBSOURCES_FILES
        bigmat.cxx
        bundle.cxx
        CB_CSolver.cxx
        CBSolver.cxx
        CFunction.cxx
        coeffmat.cxx
        coneproblem.cxx
        diagscaling.cxx
        diagtrscaling.cxx
        fullscaling.cxx
        fulltrscaling.cxx
        funproblem.cxx
        hkweight.cxx
        idscaling.cxx
        lmaxproblem.cxx
        lowrankscaling.cxx
        lowrankSMscaling.cxx
        lowranktrscaling.cxx
        lowranktrSMscaling.cxx
        MatCBSolver.cxx
        MatConcavefun.cxx
        MatConefun.cxx
        MatFCBSolver.cxx
        MatLPBCfun.cxx
        MatLPfun.cxx
        MatNBSolver.cxx
        MatSDPfun.cxx
        MatSOCfun.cxx
        Nbundle.cxx
        Nfunproblem.cxx
        Nsumproblem.cxx
        problem.cxx
        qp_sdpblock.cxx
        qp_solver.cxx
        socproblem.cxx
        sumproblem.cxx
   )

PREPEND(CBSOURCES_FILES "CBsources" ${CBSOURCES_FILES})

SET(MATRIX_FILES
        chol.cxx
        eigval.cxx
        indexmat.cxx
        lanczpol.cxx
        ldl.cxx
        matrix.cxx
        memarray.cxx
        nnls.cxx
        qr.cxx
        sparsmat.cxx
        sparssym.cxx
        symmat.cxx
        trisolve.cxx
   )

PREPEND(MATRIX_FILES "Matrix" ${MATRIX_FILES})


add_library(cb STATIC ${CBSOURCES_FILES} ${MATRIX_FILES})
target_include_directories(cb PUBLIC CBsources include Matrix Tools)
target_compile_features(cb PUBLIC cxx_std_98)
target_compile_options(cb PUBLIC -march=native)

target_include_directories(cb PUBLIC CBsources include Matrix Tools)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    target_compile_definitions(cb PRIVATE CONICBUNDLE_DEBUG=1)
else()
    target_compile_definitions(cb PRIVATE CONICBUNDLE_DEBUG=0)
endif ()

enable_testing()

add_executable(c_main c_main.c)
target_link_libraries(c_main cb)
add_test(c_main c_main)

add_executable(cxx_main cxx_main.cxx)
target_link_libraries(cxx_main cb)
add_test(cxx_main cxx_main)

add_executable(mat_main mat_main.cxx)
target_link_libraries(mat_main cb)
add_test(mat_main mat_main)
