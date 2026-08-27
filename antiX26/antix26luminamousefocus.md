#### Mouse focus setting in Lumina Desktop Environment ####

There is a poorly documented issue in Lumina-1.6.3 in which the setting of mouse focus at  Applications -> Settings -> Desktop Configuration -> Window Manager -> Focus Policy does not work.  Whatever you do it stays at ClickToFocus. 

That annoys me , because I like SloppyFocus( ie focus follows mouse). I decided to invstigate fixing this issue by modifying the source code of my Lumina fork. I had to ask for assistance from AI in attempting this, because of my limited experience with C++, zero experience with Qt, and lack of knowledge of the design of Lumina. That was quite some experience. About 2 weeks of an hour or so every night with questions and code snippets flying back anf forth between me and the AI Assistent. 

Neither of us fully understood how Lumina works, so there was a lot of diagnosis by experiment, and not much coding. We ended up concluding that it was impossible with Lumina's current architecture to address the problem by coding within Lumina. We then attempted to do  workarounds by configuring Fluxbox ( Lumina is built on top of Fluxbox) , and by configuring X11.  These also failed. 

The final solution, which is a workaround, not a code fix , is to start one of the WM's in antiX26 ( either Fluxbox, IceWM, or JWM wiill do), and then start Lumina within that WM using the `lumina-desktop` command.  That works perfectly, and Lumina will run with whatever MouseFocus setting was set in the initial WM. 

A more detailed account of the issue is given in the following writeup , which was written with AI assistance.

```
The Core Limitation: The lumina-desktop process acts as both the session manager and a global graphical event filter. It maps a hardcoded translucent desktop canvas (lumina-desktop-canvas) that captures and drops standard X11 pointer cross events, forcing a strict "Click-to-Focus" behavior regardless of underlying window manager configuration files.

The fluxbox-remote Barrier: Because lumina-desktop alters root atom window properties on launch, standard runtime communication channels (such as the _FLUXBOX_COMMAND IPC protocol) are blocked. This prevents post-boot automation tools like fluxbox-remote or xdotool from modifying window focus tracking structures dynamically.

The Optimum Solution: To achieve true mouse-hover focus tracking while retaining Lumina’s interface layout, the optimal architecture is booting a native Fluxbox X11 session [1] and appending lumina-desktop & directly to the user's local startup array. This decouples the focus management from the panel presentation layer perfectly.
```

No code modification to Lumina is required for this workaroud, so my antix26-lumina-x86_64.iso  file , available on sourceforge

https://sourceforge.net/projects/antix26-with-a-de/files/


  can be used with this workaround .... just dont start Lumina at the slimski login screen , start some other WM, and use it to start Lumina, as follows ..
```
lumina-desktop &
```
The Lumina desktop will start, all open windows will be preserved, and the Focus setting will be as set in the preceding WM.
 To exit , if you CTRL-C or kill the lumina process, it will drop back to the underlying WM, with all windows preserved.  You can then exit normally.  Or you can exit from Lumina using my AntiXExit Icon.

I expect that there may be other Lumina configurations than MouseFocus, which may be similarly affected. This workaround should address those too. 

I am unsure if earlier versions of Lumina are affected by this issue, but I suspect so. It has b=nothing to do with antiX ... it is a Lumina issue.
