message(" ")
message("+-------------------------------+")
message("|                               |")
message("|       C O M P U T R E E       |")
message("|   C O N F I G U R A T I O N   |")
message("|                               |")
message("+-------------------------------+")
message(" ")

# Check architecture
equals(QT_ARCH, x86_64)|equals(QT_ARCH, x86) {
    equals(QT_ARCH, x86_64) {
        architecture = "x86_64"
    } else {
        architecture = "x86"
        arch_warning = "(We do not recommend to use 32bit architecture)"
    }
    message("> Architecture : $$architecture")
    !isEmpty(arch_warning) {
        warning($$arch_warning)
    }
} else { # ARM and RISC are not supported
    error("Only usual desktop architectures are supported (x86, x86_64)")
}

# Check platform
linux|win32| macx {
    linux {
        platform = "Linux"
    }
    win32 {
        platform = "Windows"
    }
    macx {
        platform = "Mac OS"
    }
    message("> Platform     : $$platform")
} else { # Android, iOS, other Unix
    error("Only usual desktop platforms are supported (Linux/MacOS/Windows)")
}

# Check compiler
win32-msvc*|*-g++|*-clang {
    win32-msvc* {
        MSVC_VER = $$(VisualStudioVersion)
        # MSVC 2013
        equals(MSVC_VER, 13.0) {
            error( "MSVC 2013 detected. It is not supported anymore." )
        }
        # MSVC 2015
        equals(MSVC_VER, 14.0) {
            compiler = "MSVC_2015"
            comp_warning = "(MSVC 2015 detected. Please consider updating to a newer version)"
        }
        # MSVC 2017
        equals(MSVC_VER, 15.0) {
            compiler = "MSVC_2017"
        }
        # MSVC 2019
        equals(MSVC_VER, 16.0) {
            compiler = "MSVC_2019"
        }
    }
    *-g++ {
        compiler = "g++"
    }
    *-clang {
        compiler = "g++"
    }
    message("> Compiler     : $$compiler")
    !isEmpty(comp_warning) {
        warning($$comp_warning)
    }
    message("> C++ standard : c++17")
} else {
    error("Only usual desktop compiler are supported (Microsoft Visual Compiler/g++/Clang++)")
}

# Check Qt (version and shadow build)
greaterThan(QT_MAJOR_VERSION, 4) {
    message("> Qt version   : $$QT_VERSION")
    lessThan(QT_MINOR_VERSION, 9) {
        warning("(Qt version < 5.9 is not recommended.)")
    }
} else {
    error("Only modern Qt5 is supported")
}

!equals(PWD, "$${OUT_PWD}/config") {
    error("Shadow build seems to be activated, please desactivated it !")
} else {
    message("> Shadow build : disabled")
    message(" ")
}
