#version 120

///// FLAGS /////

// color for flags
uniform vec4 flagsColors[8];

///// INFO /////

// selected, visible, etc...
attribute float info;

bool andOperator(int val, int check) {
    int tmp = val / check;

    return int(tmp*int(0x80000000)) == int(0x80000000);
}

bool isVisible(float infoToCheck) {
    return !andOperator(int(infoToCheck), 2);
}

bool isSelected(float infoToCheck) {
    return andOperator(int(infoToCheck), 1);
}

int firstFlagEnable(float infoToCheck) {
    // TODO : find a fastest way to do that !

    if(infoToCheck == 0)
        return 8;

    int infoToCheckInt = int(infoToCheck);

    if(andOperator(infoToCheckInt, 1))
        return 0;
    else if(andOperator(infoToCheckInt, 4))
        return 2;
    else if(andOperator(infoToCheckInt, 8))
        return 3;
    else if(andOperator(infoToCheckInt, 16))
        return 4;
    else if(andOperator(infoToCheckInt, 32))
        return 5;
    else if(andOperator(infoToCheckInt, 64))
        return 6;
    else if(infoToCheckInt == 128)
        return 7;

    return 8;
}

bool prepareToComputeVertex(vec4 colorToUse, bool useColorAttribute) {

    if(isVisible(info)) {
        int ffe = firstFlagEnable(info);

        if(ffe != 8) {
            gl_FrontColor = flagsColors[ffe];
        } else if(useColorAttribute) {
            gl_FrontColor = colorToUse;
        } else {
            gl_FrontColor = vec4(1,1,1,1);
        }

        return true;
    } else {
        gl_Position = vec4(2.0, 2.0, 2.0, 1.0);
    }

    return false;
}
