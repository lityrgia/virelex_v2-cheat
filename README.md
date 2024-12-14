# 💢 Status: Abandoned
ModelMatrix in game is totally changed. Cheat is not working!

![изображение](https://github.com/user-attachments/assets/38dfc4f1-20e8-4a40-a26a-ac8d296e028e)

# 🚧 Beware
```The cheat works ONLY with intel intergrated graphics cards.```
I made this cheat only for educational purposes, playing with it will cause ban. It's hooking glDrawElements to get entities, and glFlush to send data to drawing process.

# ✨ Features
- ESP (players, npcs, some mobs)
- Draw Points
- Draw Lines
- Aimbot
- Hotkeys

# 🔧 How-to-use
1. Compile(x64) or download virelex.dll and overaly-acceptor.exe, also download any injector (e.g. [Extreme Injector](https://github.com/master131/ExtremeInjector)
2. Disable antivirus
3. Run stalcraft
4. Start overlay-accept.exe, wait for text *"Connecting to shared data..."* and inject virelex.dll
5. Press insert to Open/Close menu

# 💢 Bugs
1. The main bug i found is randomly attemping some coords, I’m not sure how to fix it, f anyone could please submit a pull request 🙏
2. Also, if no targets remain on the screen, the last target is drawn. This is done to avoid a flickering esp
3. When there are many entities on the screen, some of them may not be drawn. This can probably be fixed after fixing bug 1
