cd build
cmake ../
make
./pt scene_file_myscene1.sc image_myscene1_
./pt scene_file_myscene2.sc image_myscene2_
# ./pt scene_file_fish.sc image_fish
# ./pt scene_file_fish1.sc image_fish1_
# ./pt scene_file_fish2.sc image_fish2_
# ./pt scene_file_ppm.sc image_ppm
# ./pt scene_file_brdf.sc image_brdf
# ./pt scene_file_mis.sc image_mis
./pt scene_file.sc image_simple
./pt scene_file_pt.sc image_pt
cd ..
# python ppm2jpg.py