#pragma once

class KeyEvent
{
public:
    enum Key {
        K0,
        K1,
        K2,
        K3,
        K4,
        K5,
        K6,
        K7,
        K8,
        K9,
        KA,
        KAcBack, // the Back key (application control keypad)
        KAcBookmarks, // the Bookmarks key (application control keypad)
        KAcForward, // the Forward key (application control keypad)
        KAcHome, // the Home key (application control keypad)
        KAcRefresh,// the Refresh key (application control keypad)
        KAcSearch, // the Search key (application control keypad)
        KAcStop, // the Stop key (application control keypad)
        KAgain, // the Again key (Redo)
        KAltErase, // Erase-Eaze
        KQuote,
        KApplication, // the Application / Compose / Context Menu (Windows) key
        KAudioMute, // the Mute volume key
        KAudioNext, // the Next Track media key
        KAudioPlay, // the Play media key
        KAudioPrev, // the Previous Track media key
        KAudioStop, // the Stop media key
        KB,
        KBackslash,
        KBackspace,
        KBrightnessDown,
        KBrightnessUp,
        KC,
        KCalculator,
        KCancel,
        KCapsLock,
        KClear,
        KClearAgain,
        KComma,
        KComputer,
        KCopy,
        KCrSel,
        KCurrencySubUnit,
        KCurrencyUnit,
        KCut,
        KD,
        KDecimalSeparator,
        KDelete,
        KDisplaySwitch,
        KDown,
        KE,
        KEject,
        KEnd,
        KEquals,
        KEscape,
        KExecute,
        KExSel,
        KF,
        KF1,
        KF10,
        KF11,
        KF12,
        KF13,
        KF14,
        KF15,
        KF16,
        KF17,
        KF18,
        KF19,
        KF2,
        KF20,
        KF21,
        KF22,
        KF23,
        KF24,
        KF3,
        KF4,
        KF5,
        KF6,
        KF7,
        KF8,
        KF9,
        KFind,
        KG,
        KBackquate,
        KH,
        KHelp,
        KHome,
        KI,
        KInsert,
        KJ,
        KK,
        KKbdIllumDown, // the Keyboard Illumination Down key
        KKbdIllumToggle, // the Keyboard Illumination Toggle key
        KKbdIllumUp, // the Keyboard Illumination Up key
        KKeypad0,
        KKeypad00,
        KKeypad000,
        KKeypad1,
        KKeypad2,
        KKeypad3,
        KKeypad4,
        KKeypad5,
        KKeypad6,
        KKeypad7,
        KKeypad8,
        KKeypad9,
        KKeypadA,
        KKeypadAmpersand,
        KKeypadAt,
        KKeypadB,
        KKeypadBackspace,
        KKeypadBinary,
        KKeypadC,
        KKeypadClear,
        KKeypadClearEntry,
        KKeypadColon,
        KKeypadComma,
        KKeypadD,
        KKeypadDblAmpersand,
        KKeypadDblVerticalBar,
        KKeypadDecimal,
        KKeypadDivide,
        KKeypadE,
        KKeypadEnter,
        KKeypadEquals,
        KKeypadEqualsAs400,
        KKeypadExclam,
        KKeypadF,
        KKeypadFreater,
        KKeypadHash,
        KKeypadHexadecimal,
        KKeypadLeftBrace,
        KKeypadLeftParen,
        KKeypadLess,
        KKeypadMemAdd,
        KKeypadMemClear,
        KKeypadMemDivide,
        KKeypadMemMultiply,
        KKeypadMemRecall,
        KKeypadMemStore,
        KKeypadMemSubtract,
        KKeypadMinus,
        KKeypadMultiply,
        KKeypadOctal,
        KKeypadPercent,
        KKeypadPeriod,
        KKeypadPlus,
        KKeypadPlusMinus,
        KKeypadPower,
        KKeypadRightBrace,
        KKeypadRightParen,
        KKeypadSpace,
        KKeypadTab,
        KKeypadVerticalBar,
        KKeypadXor,
        KL,
        KLeftAlt,
        KLeftCtrl,
        KLeft,
        KLeftBracket,
        KLeftGui,
        KLeftShift,
        KM,
        KMail,
        KMediaSelect,
        KMenu,
        KMinus,
        KModeSwitch,
        KMute,
        KN,
        KNumlock,
        KO,
        KOper,
        KOut,
        KP,
        KPageDown,
        KPageUp,
        KPaste,
        KPause,
        KPeriod,
        KPower, // not a physical key - but some Mac keyboards do have a power key.
        KPrintScreen,
        KPrior,
        KQ,
        KR,
        KRightAlt,
        KRightCtrl,
        KReturn,
        KReturn2,
        KRightGUI, // meta
        KRight,
        KRightBracket,
        KRightShift,
        KS,
        KScrollLock,
        KSelect,
        KSemicolon,
        KSeparator,
        KSlash,
        KSleep,
        KSpace,
        KStop,
        KSysReq,
        KT,
        KTab,
        KThousandsSeparator,
        KU,
        KUndo,
        KUnknown,
        KUp,
        KV,
        KVolumeDown,
        KVolumeUp,
        KW,
        KWww,
        KX,
        KY,
        KZ,
        KAmpersand,
        KAsterisk,
        KAt,
        KCaret,
        KColon,
        KDollar,
        KExclaim,
        KGreater,
        KHash,
        KLeftParen,
        KLess,
        KPercent,
        KPlus,
        KQuestion,
        KQuoteDbl,
        KRightParen,
        KUnderscore
    };
    enum Modifier { 
        MNone, // 0 (no modifier is applicable)
        MLShift, // the left Shift key is down
        MRShift, // the right Shift key is down
        MLCtrl, // the left Ctrl (Control) key is down
        MRCtrl, // the right Ctrl (Control) key is down
        MLAlt, // the left Alt key is down
        MRAlt, // the right Alt key is down
        MLGui, // the left GUI key (often the Windows key) is down
        MRGui, // the right GUI key (often the Windows key) is down
        MNum, // the Num Lock key (may be located on an extended keypad) is down
        MCaps, // the Caps Lock key is down
        MMode, // the AltGr key is down
        MCtrl, // (KMOD_LCTRL|KMOD_RCTRL)
        MShift, // (KMOD_LSHIFT|KMOD_RSHIFT)
        MAlt, // (KMOD_LALT|KMOD_RALT)
        MGui, // (KMOD_LGUI|KMOD_RGUI)
        MRESERVED, // reserved for future use
    };
    KeyEvent(Key, unsigned modifiers = MNone, bool autoRepeat = false);
    bool isAutoRepeat() const;
    Key key() const;
    unsigned modifiers() const;
private:
    bool autoRepeat_;
    Key key_;
    unsigned modifiers_;
};
