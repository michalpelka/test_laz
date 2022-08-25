

#include <vector>
#include <iostream>
#include "laszip/laszip_api.h"

struct Point
{
    double x = 0.0f;
    double y = 0.0f;
    double z = 0.0f;
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint8_t  classsification;
};
template<typename T>
inline bool save_vector_data(const std::string& file_name, std::vector<T>& vector_data) {
    std::ofstream ofs(file_name, std::ios::binary);
    if (!ofs.good()) {
        return false;
    }
    ofs.write(reinterpret_cast<char*>(vector_data.data()), vector_data.size()* sizeof(T));
    return true;
}

int main(int argc, char* argv[])
{
    if (argc!=3){
        std::cerr << "need 2 argumenst :\n";
        std::cerr << argv[0] << " input.laz output.bin";
        std::abort();
    }
    laszip_POINTER laszip_reader;
    if (laszip_create(&laszip_reader))
    {
        fprintf(stderr,"DLL ERROR: creating laszip reader\n");
        std::abort();
    }

    const std::string file_name_in =argv[1];
    const std::string file_name_out =argv[2];

    laszip_BOOL is_compressed = 0;
    if (laszip_open_reader(laszip_reader, file_name_in.c_str(), &is_compressed))
    {
        fprintf(stderr,"DLL ERROR: opening laszip reader for '%s'\n", file_name_in.c_str());
        std::abort();
    }
    std::cout << "compressed : " << is_compressed << std::endl;
    laszip_header* header;

    if (laszip_get_header_pointer(laszip_reader, &header))
    {
        fprintf(stderr,"DLL ERROR: getting header pointer from laszip reader\n");
        std::abort();
    }
    fprintf(stderr,"file '%s' contains %u points\n", file_name_in.c_str(), header->number_of_point_records);
    laszip_point* point;
    if (laszip_get_point_pointer(laszip_reader, &point))
    {
        fprintf(stderr,"DLL ERROR: getting point pointer from laszip reader\n");
        std::abort();
    }
//    int64_t point_count;
    std::vector<Point> binary_points;
    binary_points.reserve(header->number_of_point_records);
    for (int i =0; i <  header->number_of_point_records; i++)
    {

        if (laszip_read_point(laszip_reader))
        {
            fprintf(stderr,"DLL ERROR: reading point %u\n", i);
            std::abort();
        }
        Point p;
        p.x = header->x_offset+ header->x_scale_factor*static_cast<double>(point->X);
        p.y = header->y_offset+ header->y_scale_factor*static_cast<double>(point->Y);
        p.z = header->z_offset+ header->z_scale_factor*static_cast<double>(point->Z);

        p.r = point->rgb[0];
        p.g = point->rgb[1];
        p.b = point->rgb[2];
        p.classsification = point->classification;

        binary_points.push_back(p);

    }
    save_vector_data<Point>(file_name_out, binary_points);
}