# Mandatory dependency check function
defineTest(checkMandatoryDependence) {
    include(default_path_$${1}.pri)
    include_file = "default_path_$${1}.pri"

    exists(user_path_$${1}.pri) {
        include(user_path_$${1}.pri)
        include_file = "user_path_$${1}.pri"
    }

    include(check_$${1}.pri)

    !isEmpty(CHECK_ERROR_MSG) {
        error("DEPENDENCY CHECK - $$name - Invalid path specified ($$include_file)")
        error($$CHECK_ERROR_MSG)
    }

    CHECK_LIBS_ONLY = true
    include(include_necessary_$${1}.pri)
    CHECK_LIBS_ONLY = false

    !isEmpty(CHECK_ERROR_MSG) {
        error("DEPENDENCY CHECK - $$upper($${1}) - Libraries not found in specified path ($$include_file)")
        error($$CHECK_ERROR_MSG)
    } else {
        message("DEPENDENCY CHECK - $$upper($${1}) - OK ($$include_file)")
    }
}

# Default relative path for libs
isEmpty(LIB_PATH) : LIB_PATH = # empty

VCPKG_PATH = $$(VCPKG)
VCPKG_PATH ~= s,/,\\,g

# Check mandatory libraries
#checkMandatoryDependence(dgtal)
checkMandatoryDependence(flann)
#checkMandatoryDependence(qhull)
#checkMandatoryDependence(gsl)
