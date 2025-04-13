#include "modules/param_editor.hpp"

template<typename T>
ParamFile<T> read_param_file(const std::string& buffer){
    ParamFile<T> param_file;
    std::memcpy(&param_file.header,buffer.data(),sizeof(ParamHeader));
    auto n_rows = param_file.header.n_rows;
    param_file.row_info.resize(n_rows);
    std::memcpy(param_file.row_info.data(),buffer.data()+sizeof(ParamHeader),sizeof(ParamRowInfo)*n_rows);
    param_file.data.resize(n_rows);
    std::memcpy(param_file.data.data(),buffer.data()+param_file.header.start_of_data,sizeof(T)*n_rows);
    // std::cout<<"Successfully read param file\n";
    // std::cout<<buffer.size()<<'\n';
    return param_file;
}
template<typename T>
std::string write_param_file(ParamFile<T>& param_file){
    if(param_file.data.size()!=param_file.row_info.size()){
        std::cout<<"Size of param file rows and data does not match\n";
        return "";
    }
    auto start_of_data = sizeof(ParamHeader)+sizeof(ParamRowInfo)*param_file.row_info.size();
    auto end_of_file = start_of_data+sizeof(T)*param_file.data.size();
    auto& header  = param_file.header;
    header.n_rows = param_file.row_info.size();
    header.end_of_file   = end_of_file;
    header.start_of_data = start_of_data;
    std::string buffer;
    buffer.resize(end_of_file,0);
    std::memcpy(buffer.data(),&param_file.header,sizeof(ParamHeader));
    for(size_t i = 0;i<param_file.row_info.size();i++){
        auto& row_entry = param_file.row_info[i];
        row_entry.end_of_file=end_of_file;
        row_entry.initial_byte=start_of_data+sizeof(T)*i;
    }

    std::memcpy(buffer.data()+sizeof(ParamHeader),param_file.row_info.data(),sizeof(ParamRowInfo)*param_file.row_info.size());
    std::memcpy(buffer.data()+start_of_data,param_file.data.data(),sizeof(T)*param_file.data.size());
    buffer+='\0';
    buffer+='\0';
    buffer+='\0';
    buffer+='\0';
    // std::cout<<"Successfully wrote param file\n";
    return buffer;
}
template<typename T>
void add_entry(u64 row,T entry,ParamFile<T>& params){
    params.row_info.push_back(ParamRowInfo{row});
    params.data.push_back(entry);
}
template<typename T>
size_t insert_next_free_entry(u64 row,T entry,ParamFile<T>& params){
    size_t offset = 0;
    for(size_t i = 0;i<params.row_info.size();i++){
        if(params.row_info[i].row>=row){
            if(params.row_info[i].row!=row+offset){
                params.data.insert(params.data.begin()+i,entry);
                params.row_info.insert(params.row_info.begin()+i,ParamRowInfo{row+offset});
                return row+offset;
            }
            offset+=1;
        }
    }
    return -1u;
}
template<typename T>
void delete_entry(size_t index,ParamFile<T>& params){
    if(index>=params.data.size()){
        std::cout<<"Deleting outside range: "<<index<<" "<<params.header.filename<<'\n';
        return;
    }
    params.row_info.erase(params.row_info.begin()+index);
    params.data.erase(params.data.begin()+index);
}
