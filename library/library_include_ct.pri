##### COMPUTREE Libraries ####

defineReplace(searchDependencies) {
    LIST = 1 2 3 4 5 6 # maximum level of dependencies

    for(i, LIST) {

        NEW_LIBS_INCLUDED = false

        for(a, COMPUTREE) {
            DEP_FILE_PATH = ../library/$${a}/libdependencies.pri

            !contains(DEPEND_FILE_INCLUDED, $$DEP_FILE_PATH) {
                exists($$DEP_FILE_PATH) {
                    DEPEND_FILE_INCLUDED += $$DEP_FILE_PATH
                    include($$DEP_FILE_PATH)
                    NEW_LIBS_INCLUDED = true
                }
            }
        }

        equals(NEW_LIBS_INCLUDED, false) {
            return($$COMPUTREE)
        }
    }

    error(Too much dependencies - please contact the developper to resolve the file include_ct_library.pri at line 4)
}

isEmpty(CT_PREFIX_LIB) {
    CT_PREFIX_LIB = ..
}

isEmpty(EXECUTABLE_DESTDIR) {
    error(EXECUTABLE_DESTDIR no defined !)
}

!isEmpty( COMPUTREE ) {
    INCLUDEPATH *= $$CT_PREFIX_LIB
    LIBS *= -L$${EXECUTABLE_DESTDIR}/libraries/core

    COMPUTREE = $$searchDependencies()

    for(b, COMPUTREE) {
#        !isEmpty(DONT_CHECK_CT_LIBRARY) {
            LIBS *= -l$${b}
            INCLUDEPATH *= $${CT_PREFIX_LIB}/$${b}
#           warning(Computree library $${b} will be used if founded)
#        } else {
#            exists(../$${CT_DESTDIR_NAME}/$${b}.*) {
#                LIBS += -l$${b}
#                warning(Computree library $${b} will be used)
#            } else {
#                error(Computree library \"$${b}\" not found. You must compile the project \"$${CT_PREFIX}/library/$${b}/$${b}.pro\" to avoid this error.)
#            }
#        }
    }
}
