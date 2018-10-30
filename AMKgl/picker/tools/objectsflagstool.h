#ifndef OBJECTSFLAGSTOOL_H
#define OBJECTSFLAGSTOOL_H

#include <QObject>

/**
 * @brief Use static method of this class to check if a point
 *        is selected or not, to select/unselect a point.
 */
class ObjectsFlagsTool
{
    Q_GADGET
    Q_ENUMS(Flag)

public:
    enum Flag {
        F_Selected = 1,
        F_Visible = 2,
        F_DevelopperFlag1 = 4,
        F_DevelopperFlag2 = 8,
        F_DevelopperFlag3 = 16,
        F_DevelopperFlag4 = 32,
        F_DevelopperFlag5 = 64,
        F_Reserved = 128
    };

    enum UnFlag {
        UF_Unselect = 254,
        UF_Unhide = 253
    };

    enum CheckFlag {
        CF_VisibleAndSelected = 3,
        CF_OneDevelopperFlag = 124
    };

    ObjectsFlagsTool();

    /**
     * @brief Select the object
     */
    template<typename T>
    static inline void staticSelectObject(T& info) { info |= F_Selected; }

    /**
     * @brief Check if must test the object when select it
     */
    template<typename T>
    static inline bool staticCheckSelectObject(const T& info) { return !(info & CF_VisibleAndSelected); }

    /**
     * @brief Unselect the object
     */
    template<typename T>
    static inline void staticUnselectObject(T& info) { info &= UF_Unselect; }

    /**
     * @brief Check if must test the object when unselect it
     */
    template<typename T>
    static inline bool staticCheckUnselectObject(const T& info) { return staticIsObjectSelected(info); }

    /**
     * @brief Toggle the object selection
     */
    template<typename T>
    static inline void staticToggleObjectSelection(T& info) { info ^= F_Selected; }

    /**
     * @brief Returns true if the object is selected
     */
    template<typename T>
    static inline bool staticIsObjectSelected(const T& info) { return info & F_Selected; }

    /**
     * @brief Hide the object
     */
    template<typename T>
    static inline void staticHideObject(T& info) { info |= F_Visible; }

    /**
     * @brief Hide the object and unselect it
     */
    template<typename T>
    static inline void staticHideObjectAndUnselectIt(T& info) { info |= F_Visible; staticUnselectObject(info); }

    /**
     * @brief Unhide (set visible) the object
     */
    template<typename T>
    static inline void staticShowObject(T& info) { info &= UF_Unhide; }

    /**
     * @brief Toggle the object visibility
     */
    template<typename T>
    static inline void staticToggleObjectVisibility(T& info) { info ^= F_Visible; }

    /**
     * @brief Toggle the object visibility and unselect it if it was invisible
     */
    template<typename T>
    static inline void staticToggleObjectVisibilityAndUnselectItIfInvisible(T& info)
    {
        info ^= F_Visible;

        if(info & F_Visible)
            staticUnselectObject(info);
    }

    /**
     * @brief Returns true if the object is visible
     */
    template<typename T>
    static inline bool staticIsObjectVisible(const T& info) { return !(info & F_Visible); }

    /**
     * @brief Set flags ON
     */
    template<typename T>
    static inline void staticSetFlags(T& info, const quint8& flags) { info |= flags; }

    /**
     * @brief Set flags OFF
     */
    template<typename T>
    static inline void staticUnsetFlags(T& info, const quint8& flags) { info &= (255-flags); }

    /**
     * @brief Toggle flags
     */
    template<typename T>
    static inline void staticToggleFlags(T& info, const quint8& flags) { info ^= flags; }

    /**
     * @brief Returns true if flags is ON
     */
    template<typename T>
    static inline bool staticIsFlagsSet(const T& info, const quint8& flags) { return (quint8(info) & flags) == flags; }

    /**
     * @brief Returns true if one flag is ON
     */
    template<typename T>
    static inline bool staticIsAtLeastOneFlagSet(const T& info, const quint8& flags) { return info & flags; }

    /**
     * @brief Set the flag ON
     */
    template<typename T>
    static inline void staticSetFlag(T& info, const ObjectsFlagsTool::Flag& flag) { staticSetFlags(info, flag); }

    /**
     * @brief Set the flag OFF
     */
    template<typename T>
    static inline void staticUnsetFlag(T& info, const ObjectsFlagsTool::Flag& flag) { staticUnsetFlags(info, flag); }

    /**
     * @brief Toggle the flag
     */
    template<typename T>
    static inline void staticToggleFlag(T& info, const ObjectsFlagsTool::Flag& flag) { staticToggleFlags(info, flag); }

    /**
     * @brief Returns true if the flag is ON
     */
    template<typename T>
    static inline bool staticIsFlagSet(const T& info, const ObjectsFlagsTool::Flag& flag) { return staticIsFlagsSet(info, flag); }

    /**
     * @brief Returns the first flag set
     */
    template<typename T>
    static inline ObjectsFlagsTool::Flag staticFirstFlagSet(T info) {
        return ObjectsFlagsTool::Flag(info & ((~info) + 1));
    }

    /**
     * @brief Returns the first developper's flag set
     */
    template<typename T>
    static inline ObjectsFlagsTool::Flag staticFirstDevelopperFlagSet(const T& info) {
        const quint8 devInfo = info & CF_OneDevelopperFlag;

        return staticFirstFlagSet(devInfo);
    }
};

#endif // OBJECTSFLAGSTOOL_H
