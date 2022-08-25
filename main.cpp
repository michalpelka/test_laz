

#include <vector>
#include <iostream>
#include "laszip/laszip_api.h"

int main(int argc, char* argv[])
{
    laszip_POINTER laszip_reader;
    if (laszip_create(&laszip_reader))
    {
        fprintf(stderr,"DLL ERROR: creating laszip reader\n");
        std::abort();
    }

    std::string file_name_in ="/media/michal/ext/orto_photo_skierniewice/skierniewice/70120_819492_M-34-5-A-b-1-3-2-3.laz";
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

    std::ofstream txt("/tmp/test.asc");
    for (int i =0; i <  header->number_of_point_records; i++)
    {

        if (laszip_read_point(laszip_reader))
        {
            fprintf(stderr,"DLL ERROR: reading point %u\n", i);
            std::abort();
        }

        txt << point->X << "\t"<< point->Y << "\t"<< point->Z << "\n";
    }


//    std::ifstream ifs;
//    ifs.open("/media/michal/ext/orto_photo_skierniewice/skierniewice/70120_819492_M-34-5-A-b-1-3-2-3.laz",  std::ios::in | std::ios::binary);
//    liblas::ReaderFactory f;
//    liblas::Reader reader = f.CreateWithStream(ifs);
//    liblas::Header const& header = reader.GetHeader();
//
//    std::cout << "Compressed: " << (header.Compressed() == true) ? "true":"false";
//    std::cout << "Signature: " << header.GetFileSignature() << '\n';
//    std::cout << "Points count: " << header.GetPointRecordsCount() << '\n';

}