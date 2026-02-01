#pragma once

#include <string>
#include <vector>
#include <stdexcept>

/*
* Generic text-based formatted table utility.
* Tables are started with columns headers seperated by pipes ('|') 
* and a separator consisting of dashes and a plus ('+') symbol to denote a intersection between the pipes and dashes.
* The cells are center justified, and odd column widths contents prefer to lean left.
* The column width is determined by padding a space on either end of the LONGEST cell of the entire column.
*/
class FormattedTable
{
public:
    explicit FormattedTable(std::vector<std::string> headers);
    void addRow(const std::vector<std::string>& row);
    std::string format(int highlightRow = -1) const; // index of the row to select, -1 = no highlight

private:
    using Row = std::vector<std::string>;
    std::vector<std::string> headers;
    std::vector<Row> rows;
    mutable std::vector<size_t> col_widths; // cached column widths

    void computeColumnWidths() const;
    std::string formatRow(const Row& row) const;
    std::string separatorLine() const;
    std::string centerText(const std::string& text, size_t width) const;
};
