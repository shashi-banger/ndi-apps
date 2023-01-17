
## Build sample apps

The following works on x86 mac

```bash
gcc NDIlib_Send_Video.cpp -o ndi-send -I/Library/NDI\ SDK\ for\ Apple/include -lstdc++ -lndi -L/Library/NDI\ SDK\ for\ Apple/lib/macOS/
gcc NDIlib_Send_Video_Advanced.cpp -o ndi-send-adv -I/Library/NDI\ SDK\ for\ Apple/include -lstdc++ -lndi -L/Library/NDI\ SDK\ for\ Apple/lib/macOS/
gcc NDIlib_Recv.cpp -o ndi-recv -I/Library/NDI\ SDK\ for\ Apple/include -lstdc++ -lndi -L/Library/NDI\ SDK\ for\ Apple/lib/macOS/

# Terminal-1

export LD_LIBRARY_PATH=/Library/NDI\ SDK\ for\ Apple/lib/macOS
./ndi-send


# Terminal-2
export LD_LIBRARY_PATH=/Library/NDI\ SDK\ for\ Apple/lib/macOS
./ndi-recv

```
