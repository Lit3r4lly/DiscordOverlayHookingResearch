# DiscordOverlayHookingResearch

This research is about Discord overlay hooking library with implementation of written abuse module which made an "hook to hook" situation.  
Because Discord's hook module (DiscordHook64.dll) uses the Minhook library, including Pattern scanning technique we can find the functions addresses (minhook calls).  
After we get these addresses we can call these functions and hook the original functions. 
Bottom line, we getting code execution by the discord hook module which make us available to make cheats undetectable (couple of AC's already patched the Discord's overlay feature and block this..).

## Remarks

- Build only for x64 bit (debug or release mode)
- Inject only into processes that are x64 bit

## Usage

```
1) Build the dll
2) Inject the dll to the target
Enjoy!
```

## What is Pattern Scanning

Pattern Scanning is a technique which made for extracting offsets from modules contents and more specifically.
We are implementing this technique by looking for the surrounding instructions code (bytes) and by that getting the function address.

**For pattern scanning we need two main elements:**  
`Pattern` - a pattern is a number of instructions that are represented by a byte sequence and includes a specific r/m64 instruction that contains the offset as mentioned above.  
`Mask` -  a mask defines which bytes of your pattern are wildcards and which are not, by using the characters '?' and 'x'.  
The character 'x' means 'byte must match with the one from the module content' and  the character '?' means 'the index of where we can find the offset in the module content respectively with the pattern'  

**For example:**  
Pattern - `\xA1\x00\x00\x00\x00\x33\xD2\x6A\x00\x6A\x00\x33\xC9\x89\xB0`  
Mask -  `x????xxxxxxxxxx`  

## Build

DiscordOverlyHooking can be built with Visual Studio 2019, by opening the .sin file and build the project (Ctrl+Shift+B) in a Release Mode or Debug Mode, whatever you want to (x64)

## Issues

If you have any issues with this tool, you can ping me on Discord: Lit3r4lly#8336  
If you have some critical bug, open an PR/Issue ticket
