/**
 * @file     dataframe.h
 * @class    dataframe
 * @brief    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *           read from csv file
 *           write into csv file
 *           append one row from std::vector & remove row
 *           insert one column from std::vector & remove column
 *           get a row of data by index of the row
 *           get a column of data by string of the column
 *           concat & add double dataFrame object (horizontally & vertically)
 *           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * @details
 * @author   Flame
 * @date     10.01.2020
**/

#ifndef DATAFRAME_H
#define DATAFRAME_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <sstream>
#include <unordered_map>

template <typename T = double>
class dataframe
{
public:
    class column_array
    {
        typedef typename std::vector<T>::const_iterator iter;
        std::vector<T> * _array = nullptr;
    public:
        explicit column_array(int n = 0){
            _array = new std::vector<T>(n);
        }

        column_array (const column_array & __array){
            _array = new std::vector<T>(*__array._array);
        }

        column_array (const column_array && __array) noexcept {
            _array = new std::vector<T>(std::move(*__array._array));
        }

        explicit column_array (const std::vector<T> && __array){
            _array = new std::vector<T>(__array);
        }

        explicit column_array (const std::vector<T> & __array){
            _array = new std::vector<T>(__array);
        }

        ~column_array(){
            delete _array;
        }

        void insert(iter position,iter start,iter end){
            _array->insert(position,start,end);
        }

        [[nodiscard]] unsigned long long int size() const{
            if(_array == nullptr)
                return 0;
            return _array->size();
        }

        [[nodiscard]] iter begin() const{
            return _array->begin();
        }

        [[nodiscard]] iter end() const{
            return _array->end();
        }

        void erase(iter i){
            _array->erase(i);
        }
        void emplace_back(const T & item){
            _array->emplace_back(item);
        }

        column_array & operator = (const column_array & __array){
            if(__array.size()==_array->size()){
                _array->clear();
                _array->insert(_array->begin(),__array.begin(),__array.end());
                return *this;
            }
            throw (std::invalid_argument("The length of the two is not the same"));
        }

        column_array & operator = (const std::vector<T> & __array){
            if(__array.size()==_array->size()){
                _array->clear();
                _array->insert(_array->begin(),__array.begin(),__array.end());
                return *this;
            }
            throw (std::invalid_argument("The length of the two is not the same"));
        }

        const T & operator [](unsigned long long int i) const {
            if(i < _array->size())
                return (*_array)[i];
            else{
                std::stringstream ssTemp;
                ssTemp << i;
                throw (std::out_of_range("the index \'" + ssTemp.str() + "\' is out of range!"));
            }
        }

        T & operator [](unsigned long long int i) {
            if(i < _array->size())
                return (*_array)[i];
            else{
                std::stringstream ssTemp;
                ssTemp << i;
                throw (std::out_of_range("the index \'" + ssTemp.str() + "\' is out of range!"));
            }
        }

        friend std::ostream &operator<<(std::ostream &cout, column_array & arr) {
            for(const auto & item : *arr._array){
                cout << item << ' ';
            }
            return cout;
        }
    };

    class row_array
    {
        typedef typename std::vector<T *>::const_iterator iter;
        std::vector<T *> * _array = nullptr;
    public:
        explicit row_array(int n = 0){
            _array = new std::vector<T*>(n);
        }

        row_array (const row_array & __array){
            _array = new std::vector<T*>(*__array._array);
        }

        row_array (const row_array && __array) noexcept {
            _array = new std::vector<T*>(std::move(*__array._array));
        }

        explicit row_array (const std::vector<T *> && __array){
            _array = new std::vector<T*>(__array);
        }

        explicit row_array (const std::vector<T *> & __array){
            _array = new std::vector<T*>(__array);
        }

        ~row_array(){
            delete _array;
        }

        void insert(iter position,iter start,iter end){
            _array->insert(position,start,end);
        }

        [[nodiscard]] unsigned long long int size() const{
            if(_array == nullptr)
                return 0;
            return _array->size();
        }

        [[nodiscard]] iter begin() const{
            return _array->begin();
        }

        [[nodiscard]] iter end() const{
            return _array->end();
        }

        row_array & operator = (const row_array & __array){
            if(__array.size()==_array->size()){
                for (int i = 0; i < __array.size(); ++i) {
                    *(*_array)[i] = __array[i];
                }
                return *this;
            }
            throw (std::invalid_argument("The length of the two is not the same"));
        }

        row_array & operator = (const std::vector<T> & __array){
            if(__array.size()==_array->size()){
                for (int i = 0; i < __array.size(); ++i) {
                    *(*_array)[i] = __array[i];
                }
                return *this;
            }
            throw (std::invalid_argument("The length of the two is not the same"));
        }

        void push_back(T * item){
            _array->emplace_back(item);
        }

        const T & operator [](unsigned long long int i) const {
            if(i < _array->size())
                return *((*_array)[i]);
            else{
                std::stringstream ssTemp;
                ssTemp << i;
                throw (std::out_of_range("the index \'" + ssTemp.str() + "\' is out of range!"));
            }
        }

        T & operator [](unsigned long long int i) {
            if(i < _array->size())
                return *((*_array)[i]);
            else{
                std::stringstream ssTemp;
                ssTemp << i;
                throw (std::out_of_range("the index \'" + ssTemp.str() + "\' is out of range!"));
            }
        }

        friend std::ostream &operator<<(std::ostream &cout, row_array & arr) {
            for(const auto & item : *arr._array){
                cout << item << ' ';
            }
            return cout;
        }
    };

private:
    typedef std::vector<column_array *> Matrix;
    typedef std::vector<std::string> Column;
    typedef std::vector<std::string> StringVector;

public:
    explicit dataframe(const std::string & filename, const char & delimiter=','): width(0), length(0){
        read_csv(filename,delimiter);
    }

    explicit dataframe(long long int _width, long long int _length = 0): width(_width), length(_length){
        Column temp;
        for(int i = 0; i < width; i++)
            temp.emplace_back(std::to_string(i));
        column_paste(temp);
    }

    explicit dataframe(const Column& columns): width(columns.size()), length(0){
        column_paste(columns);
    }

    dataframe(const dataframe & dataframe):
            width(dataframe.width),
            length(dataframe.length),
            column(dataframe.column),
            index(dataframe.index){
        matrix.clear();
        for (auto i = dataframe.matrix.begin(); i < dataframe.matrix.end(); ++i) {
            matrix.emplace_back(new column_array(**i));
        }
    }

    dataframe(const dataframe && dataframe) noexcept :
        width(dataframe.width),
        length(dataframe.length),
        column(std::move(dataframe.column)),
        index(std::move(dataframe.index)){
        matrix.clear();
        for (auto i = dataframe.matrix.begin(); i < dataframe.matrix.end(); ++i) {
            matrix.emplace_back(new column_array(std::move(**i)));
        }
    }

    ~dataframe(){
        for(auto i = 0; i < matrix.size(); ++ i){
            delete matrix[i];
        }
    }

    // determine whether the column is included
    bool contain(const std::string & col){
        return index.find(col)!=index.end();
    }

    // insert one column
    bool insert(const std::string & col){
        ++ width;
        column.emplace_back(col);
        index.emplace(col,index.size());
        matrix.emplace_back(new column_array(length));
        return true;
    }

    // insert one column from std::vector<T>
    bool insert(const std::string & col,const column_array && _array){
        if(_array.size() == row_num()){
            if(!contain(col)){
                ++ width;
                column.emplace_back(col);
                index.emplace(col,index.size());
                matrix.emplace_back(new column_array(_array));
            }else{
                column_array & line = this->operator[](col);
                if(line.size() == _array.size()){
                    line = _array;
                }
                else throw (std::invalid_argument("The length of the two is not the same"));
            }
            return true;
        }else return false;
    }

    // insert one column from std::vector<T>
    bool insert(const std::string & col,const column_array & _array){
        if(_array.size() == row_num()){
            if(!contain(col)){
                ++ width;
                column.emplace_back(col);
                index.emplace(col,index.size());
                matrix.emplace_back(new column_array(_array));
            }else{
                column_array & line = this->operator[](col);
                if(line.size() == _array.size()){
                    line = _array;
                }
                else throw (std::invalid_argument("The length of the two is not the same"));
            }
            return true;
        }else return false;
    }

    // insert one column from std::vector<T>
    bool insert(const std::string & col,const std::vector<T> && _array){
        if(_array.size() == row_num()){
            if(!contain(col)){
                ++ width;
                column.emplace_back(col);
                index.emplace(col,index.size());
                matrix.emplace_back(new column_array(_array));
            }else{
                column_array & line = this->operator[](col);
                if(line.size() == _array.size()){
                    line = _array;
                }
                else throw (std::invalid_argument("The length of the two is not the same"));
            }
            return true;
        }else return false;
    }

    // insert one column from std::vector<T>
    bool insert(const std::string & col,const std::vector<T> & _array){
        if(_array.size() == row_num()){
            if(!contain(col)){
                ++ width;
                column.emplace_back(col);
                index.emplace(col,index.size());
                matrix.emplace_back(new column_array(_array));
            }else{
                column_array & line = this->operator[](col);
                if(line.size() == _array.size()){
                    line = _array;
                }
                else throw (std::invalid_argument("The length of the two is not the same"));
            }
            return true;
        }else return false;
    }

    //remove one column from str
    bool remove(const std::string & col){
        auto item = index.find(col);
        if(item != index.end()){
            -- width;
            column.erase(column.begin()+item->second);
            delete *(matrix.begin()+item->second);
            matrix.erase(matrix.begin()+item->second);
            for(auto & index_item : index){
                if(index_item.second > item->second){
                    index_item.second--;
                }
            }
            index.erase(item);
            return true;
        }
        return false;
    }

    //remove one row from index
    bool remove(int i){
        if(i < length){
            for(auto & item : matrix){
                item->erase(item->begin()+i);
            }
            --length;
            return true;
        }else {
            return false;
        }
    }

    //get one row data from index of row
    row_array operator [](int i){
        if(i < length){
            row_array row_array;
            for(auto & item : matrix){
                row_array.push_back(&((*item)[i]));
            }
            return std::move(row_array);
        }else {
            std::stringstream ssTemp;
            ssTemp << i;
            throw (std::out_of_range("the index \'" + ssTemp.str() + "\' is out of range!"));
        }
    }

    //get one row data from index of row
    const row_array & operator [](int i) const{
        if(i < length){
            row_array row_array;
            for(auto & item : matrix){
                row_array.push_back(&((*item)[i]));
            }
            return std::move(row_array);
        }else {
            std::stringstream ssTemp;
            ssTemp << i;
            throw (std::out_of_range("the index \'" + ssTemp.str() + "\' is out of range!"));
        }
    }

    //get one column data from column str
    column_array & operator [](const std::string & col){
        auto item = index.find(col);
        if(item != index.end()){
            return *(matrix[item->second]);
        }
        insert(col);
        return *(matrix.back());
    }

    //get one column data from column str
    const column_array & operator [](const std::string & col) const{
        auto item = index.find(col);
        if(item != index.end()){
            return *(matrix[item->second]);
        }
        insert(col);
        return *(matrix.back());
    }

    //append one row from std::vector<T>
    bool append(const std::vector<T> & _array){
        if(_array.size() == width){
            length ++;
            for (int i = 0; i < _array.size(); ++i) {
                matrix[i]->emplace_back(_array[i]);
            }
            return true;
        }else return false;
    }

    //append one row from std::vector<T>
    bool append(const std::vector<T> && _array){
        if(_array.size() == width){
            length ++;
            for (int i = 0; i < _array.size(); ++i) {
                matrix[i]->emplace_back(std::move(_array[i]));
            }
            return true;
        }else return false;
    }

    [[nodiscard]] const long long int & column_num() const {
        return width;
    }

    [[nodiscard]] const long long int & row_num() const {
        return length;
    }

    //concat double dataframe object vertically
    bool concat_line(const dataframe & dataframe){
        if(dataframe.width==width){
            length += dataframe.length;
            for (int i = 0; i < width; ++i) {
                matrix[i]->insert(matrix[i]->end(),dataframe.get_column(i).begin(),dataframe.get_column(i).end());
            }
            return true;
        }else return false;
    }

    //concat double dataframe object horizontally
    bool concat_row(const dataframe & dataframe){
        if(dataframe.length==length){
            std::string repeat;
            auto last_width = dataframe.width;
            for (int i = 0; i < last_width; ++i) {
                repeat = contain(dataframe.column[i]) ? "_r" : "";
                index.insert({dataframe.column[i] + repeat,index.size()});
                column.emplace_back(dataframe.column[i] + repeat);
                matrix.emplace_back(new column_array(*dataframe.matrix[i]));
            }
            width += dataframe.column_num();
            return true;
        }else return false;
    }

    //concat double dataframe object horizontally
    bool concat_row(const dataframe && dataframe){
        if(dataframe.length==length){
            std::string repeat;
            auto last_width = dataframe.width;
            for (int i = 0; i < last_width; ++i) {
                repeat = contain(dataframe.column[i]) ? "_r" : "";
                index.insert({dataframe.column[i] + repeat,index.size()});
                column.emplace_back(dataframe.column[i] + repeat);
                matrix.emplace_back(new column_array(std::move(*dataframe.matrix[i])));
            }
            width += dataframe.column_num();
            return true;
        }else return false;
    }

    // is empty or not
    bool empty() const {
        return width == 0;
    }

    //concat double dataframe object vertically
    friend dataframe operator + (const dataframe & dataframe1, const dataframe & dataframe2){
        if(!dataframe1.empty() && !dataframe2.empty() &&
           dataframe1.column_num() == dataframe2.column_num()){
            dataframe dataFrame(dataframe1.column);
            dataFrame.concat_line(dataframe1);
            dataFrame.concat_line(dataframe2);
            return std::move(dataFrame);
        } else throw (std::invalid_argument("The column length of the two is not the same"));
    }

    //write into csv file
    void to_csv(const std::string & filename, const char & delimiter = ',') const {
        std::ofstream cout = std::ofstream(filename.data(), std::ios::out | std::ios::trunc);
        for (auto item = column.begin(); item < column.end() - 1; ++item) {
            cout << *item << delimiter;
        }
        cout << column.back() << '\n';
        for (int i = 0; i < row_num(); ++i) {
            for (auto _array = matrix.begin(); _array < matrix.end() - 1; ++_array) {
                cout << (**_array)[i] << delimiter;
            }
            cout << (*matrix.back())[i] << '\n';
        }
        cout.close();
    }

    //print dataframe
    friend std::ostream &operator<<(std::ostream & cout, const dataframe &dataFrame) {
        cout << "width : " << dataFrame.width << std::endl;
        cout << "length : " << dataFrame.length << std::endl;
        for (int j = 0; j < dataFrame.width; ++j) {
            cout << dataFrame.column[j] << "\t";
        }
        cout << '\n';
        for (int i = 0; i < dataFrame.length; ++i) {
            for (int j = 0; j < dataFrame.width; ++j) {
                cout << (*dataFrame.matrix[j])[i] << "\t";
            }
            cout << '\n';
        }
        return cout;
    }

    // get name vector of columns
    const std::vector<std::string> & get_column_str(){
        return column;
    }

private:
    //init the column
    bool column_paste(const Column & _column){
        if(!_column.empty()) {
            width = _column.size();
            column.clear();
            index.clear();
            matrix.clear();
            for(const auto & item : _column){
                column.emplace_back(item);
                index.emplace(item,index.size());
                matrix.emplace_back(new column_array(0));
            }
            return true;
        } else return false;
    }

    // print name of column
    void show_columns(){
        for(const auto & item : index){
            std::cout << item.first << ',' << item.second << std::endl;
        }
    }

    //read from csv file
    void read_csv(const std::string & filename, const char & delimiter = ',') {
        bool convert = false;
        std::ifstream Reader(filename.data());
        if(!Reader){
            throw (std::invalid_argument(filename + " is invalid!"));
        }

        std::string strLine;
        StringVector vecValue;
        if(getline(Reader, strLine)){
            vecValue.clear();
            if(splite_line(strLine, vecValue ,delimiter)) {
                column_paste(vecValue);
            }
        }

        while (getline(Reader, strLine)){
            vecValue.clear();
            if(splite_line(strLine, vecValue ,delimiter)) {
                append_from_str(vecValue);
            }
        }
        Reader.close();
    }

    //get one column data from index of column
    const column_array & get_column (const int & i) const{
        if(i < matrix.size())
            return *(matrix[i]);
        std::stringstream ssTemp;
        ssTemp << i;
        throw (std::out_of_range("the index \'" + ssTemp.str() + "\' is out of range!"));
    }

    //get one column data from index of column
    column_array & get_column(const int & i){
        if(i < matrix.size())
            return *(matrix[i]);
        std::stringstream ssTemp;
        ssTemp << i;
        throw (std::out_of_range("the index \'" + ssTemp.str() + "\' is out of range!"));
    }

    //get one row data from index of row
    row_array get_row(int i){
        if(i < length){
            row_array row_array;
            for(auto & item : matrix){
                row_array.push_back(&((*item)[i]));
            }
            return std::move(row_array);
        }else {
            std::stringstream ssTemp;
            ssTemp << i;
            throw (std::out_of_range("the index \'" + ssTemp.str() + "\' is out of range!"));
        }
    }

    //get one row data from index of row
    const row_array & get_row(int i) const{
        if(i < length){
            row_array row_array;
            for(auto & item : matrix){
                row_array.push_back(&((*item)[i]));
            }
            return std::move(row_array);
        }else {
            std::stringstream ssTemp;
            ssTemp << i;
            throw (std::out_of_range("the index \'" + ssTemp.str() + "\' is out of range!"));
        }
    }

    // separate strings by delimiter
    bool splite_line(const std::string & strLine, StringVector & stringVector, const char & delimiter) {
        unsigned long long nBegin = 0;
        unsigned long long nEnd = 0;
        unsigned int iter = 0;
        bool flag = false;
        std::string strtemp;
        while ((nEnd = strLine.find_first_of(delimiter, nBegin)) != std::string::npos)
        {
            strtemp = strLine.substr(nBegin, nEnd - nBegin);
            stringVector.emplace_back(strtemp);
            nBegin = nEnd + 1;
        }
        if ((nBegin = strLine.find_last_of(delimiter, strLine.length() - 1)) != std::string::npos)
        {
            strtemp = strLine.substr(nBegin + 1, strLine.length() - nBegin -1);
            stringVector.emplace_back(strtemp);
            flag = true;
        }
        return flag;
    }

    // append data from string vector
    bool append_from_str(const StringVector & _array){
        if(_array.size() == column.size()){
            length ++;
            std::stringstream stream;
            T item;
            for (unsigned int i = 0; i < _array.size(); ++i) {
                stream.clear();
                stream << _array[i];
                stream >> item;
                matrix[i]->emplace_back(item);
            }
            return true;
        }else return false;
    }

    Column column;
    Matrix matrix;
    long long int width;
    long long int length;
    std::unordered_map<std::string,unsigned long long int> index;
};

#endif // DATAFRAME_H
