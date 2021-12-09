.PHONY: clean bundle

soundpipe: soundpipe.c
	$(CC) -x objective-c -o soundpipe `sdl-config --cflags --libs` soundpipe.c
	install_name_tool -add_rpath @executable_path/. soundpipe
	install_name_tool -add_rpath @executable_path/../Resources/lib soundpipe
	install_name_tool -change /usr/local/opt/sdl/lib/libSDL-1.2.0.dylib @rpath/libSDL-1.2.0.dylib soundpipe
	strip soundpipe
	[ -f ../virtualt.app/Contents/MacOS/virtualt-bin ] || mv ../virtualt.app/Contents/MacOS/virtualt ../virtualt.app/Contents/MacOS/virtualt-bin
	cp virtualt-sh ../virtualt.app/Contents/MacOS/virtualt
	cp Info.plist ../virtualt.app/Contents
	mkdir ../virtualt.app/Contents/Resources >/dev/null 2>&1 || true
	cp soundpipe ../virtualt.app/Contents/Resources
	cp icon.icns ../virtualt.app/Contents/Resources
	mkdir ../virtualt.app/Contents/Resources/lib >/dev/null 2>&1 || true
	rm -f ../virtualt.app/Contents/Resources/lib/* >/dev/null 2>&1 || true
	cp /usr/local/opt/sdl/lib/libSDL-1.2.0.dylib /usr/local/opt/fltk/lib/libfltk_images.1.3.dylib /usr/local/opt/libpng/lib/libpng16.16.dylib /usr/local/opt/jpeg/lib/libjpeg.9.dylib /usr/local/opt/fltk/lib/libfltk.1.3.dylib ../virtualt.app/Contents/Resources/lib || true
	chmod -R u+rw ../virtualt.app

clean: 
	$(RM) soundpipe

