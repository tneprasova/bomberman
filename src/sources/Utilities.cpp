#include "Utilities.hpp"

int randomInt(const int & from, const int & to)
{
    using namespace std;

    random_device device;
    mt19937 engine(device());
    uniform_int_distribution<mt19937::result_type> dist(from, to);

    return dist(engine);
}

void trim(std::string & str, const std::string & charsToAvoid)
{
    str.erase(0, str.find_first_not_of(charsToAvoid));
    str.erase(str.find_last_not_of(charsToAvoid) + 1);
}

int loadData(const char * filePath, const char * item)
{
    using namespace std;

    stringstream stream = loadData(filePath, item, 1);
    int num;

    // Try to convert the stream to an int
    try { stoi(stream.str()); }
    catch (const invalid_argument & err){ throw FileException("Expected one numerical value under label "s.append(item)); }

    stream >> num;
    // In case the stream contained something more, throw an error
    if (! stream.eof())
        throw FileException("Expected one numerical value under label "s.append(item));

    return num;
}

std::stringstream loadData(const char * filePath, const char * item, const bool & flag)
{
    using namespace std;

    ifstream in(filePath);      // The input file
    string lookUp = item;       // The label to look up
    lookUp = toLower(lookUp);
    trim(lookUp);
    string line;                // Line of a file
    stringstream stream;        // The result data stream
    
    if (! in.is_open() || in.fail())
        throw FileException("Failed to open the file");
    
    string tmpLabel;
    // Read until the look up label is found
    while (! in.eof() || ! in.fail())
    {
        getline(in, line);
        tmpLabel = line;
        trim(line, " \n\t\"");

        if (toLower(line) == lookUp)
        {
            trim(line, " \n\t");
            break;
        }
    }

    // Label is not in the file
    if (in.eof() && toLower(line) != lookUp)
        throw FileException("The item "s.append(item).append(" was not found"));

    // The label was found, it was at the end of the file with no data
    if (in.eof() && isLabel(tmpLabel))
        throw FileException("No data found under label "s.append(item));
    
    if (in.fail())
        throw FileException("File could not be read");


    getline(in, line);
    trim(line);
    for (int i = 0; ; ++ i)
    {
        // Found another label - reading the data has ended
        if (isLabel(line))
            break;

        if (in.fail() && ! in.eof())
            throw FileException("Error while reading a file");
        stream << line;
        if (in.eof())
            break;

        getline(in, line);
        trim(line);

        if (! isLabel(line))
            stream << "\n";
    }
    in.close();
    
    // No data was under the label
    if (stream.str().empty())
        throw FileException("No data found under label "s.append(item));

    return stream;
}

void saveData(const char * filePath, const char * item, const int & num)
{
    std::stringstream stream("");
    stream << num;
    saveData(filePath, item, stream);
}

void saveData(const char * filePath, const char * item, const std::stringstream & data)
{
    using namespace std;

    ifstream in(filePath);
    string lookUp = item;
    lookUp = toLower(lookUp);
    trim(lookUp);
    string line;
    stringstream file;
    
    // If the file couldn't be opened, try to create it
    if (! in.is_open())
    {
        ofstream create(filePath);
        create.close();
        in.open(filePath);
        if (! in.is_open())
            throw FileException("Couldn't create a save file");
    }
    
    bool labelFound = false;
    bool fileIsEmpty =  in.peek() == std::ifstream::traits_type::eof();

    while (! in.fail())
    {
        // Read line by line, save the data into a tmp stream
        getline(in, line);
        file << line;
        trim(line, " \t\n\"");

        if (toLower(line) == lookUp)
        {
            labelFound = true;
            file << "\n";
            // Read data until another label is found, don't save it
            // into the tmp stream
            for (int i = 0;; ++ i)
            {
                getline(in, line);
                trim(line, " \n\t");
                if (in.eof() || isLabel(line))
                    break;
            }
            // Append our data which we want to save to the tmp stream
            file << data.rdbuf();

            if (isLabel(line))
                file << "\n" << line;
        }
        // Continue reading and saving rest of the data until EOF
        if (in.eof())
            break;
        file << "\n";
    }
    // Ensures that en empty file does not start with an empty line
    if (! fileIsEmpty && ! labelFound)
        file << "\n";

    // Create the label and save the data in case it was not found
    if (! labelFound)
        file << "\"" << item << "\"\n" << data.rdbuf();

    if (in.fail() && ! in.eof())
        throw FileException("File could not be read");
    in.close();

    ofstream out(filePath);
    if (! out.is_open() || out.fail())
        throw FileException("Failed to open the file");

    // Finaly write the tmp stream into the output file
    out << file.rdbuf();
    if (out.fail())
        throw FileException("File could not be rewritten");

    out.close();
}

std::string toLower(const std::string & str)
{
    std::string res = str;
    transform(res.begin(), res.end(), res.begin(), [](unsigned char c){ return tolower(c); });
    return res;
}

bool isLabel(const std::string & line)
{
    if (line.front() != '"' || line.back() != '"')
        return false;

    return true;
}