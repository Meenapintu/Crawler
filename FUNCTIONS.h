size_t write_data(void *buffer, size_t size, size_t nmemb, void *stream)
{
    string data((const char*)buffer, (size_t) size * nmemb);
    *((stringstream*) stream) << data << endl;
    return size * nmemb;
}
size_t write_data_file(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
