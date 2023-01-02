cd build
cmake ../
make
./pt scene_file_brdf.sc image_brdf
./pt scene_file_mis.sc image_mis
./pt scene_file.sc image_simple
cd ..
# python ppm2jpg.py