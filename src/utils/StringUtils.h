// utils/StringUtils.h
#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
using namespace std;

class StringUtils {
public:
    // Escape CSV field (wrap in quotes if contains comma or quote)
    static string escapeCSV(const string& field) {
        bool needsEscape = (field.find(',') != string::npos || 
                           field.find('"') != string::npos ||
                           field.find('\n') != string::npos);
        
        if (!needsEscape) {
            return field;
        }
        
        string escaped = "\"";
        for (char c : field) {
            if (c == '"') {
                escaped += "\"\"";  // Double the quotes
            }
            escaped += c;
        }
        escaped += "\"";
        return escaped;
    }
    
    // Remove quotes from CSV field
    static string unescapeCSV(const string& field) {
        if (field.length() >= 2 && field.front() == '"' && field.back() == '"') {
            string unescaped = field.substr(1, field.length() - 2);
            // Replace "" with "
            size_t pos = 0;
            while ((pos = unescaped.find("\"\"", pos)) != string::npos) {
                unescaped.replace(pos, 2, "\"");
                pos += 1;
            }
            return unescaped;
        }
        return field;
    }
    
    // Trim whitespace
    static string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == string::npos) return "";
        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }
    
    // Convert to lowercase
    static string toLower(const string& str) {
        string lower = str;
        transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
        return lower;
    }
    
    // Split string by delimiter (handles quoted CSV fields)
    static vector<string> splitCSV(const string& line) {
        vector<string> result;
        string current;
        bool inQuotes = false;
        
        for (size_t i = 0; i < line.length(); i++) {
            char c = line[i];
            
            if (c == '"') {
                // Check for escaped quote ""
                if (i + 1 < line.length() && line[i + 1] == '"') {
                    current += '"';
                    i++;  // Skip next quote
                } else {
                    inQuotes = !inQuotes;
                }
            } else if (c == ',' && !inQuotes) {
                result.push_back(trim(current));
                current.clear();
            } else {
                current += c;
            }
        }
        result.push_back(trim(current));
        return result;
    }
};

#endif // STRINGUTILS_H