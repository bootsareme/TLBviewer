#include "FormattedTable.h"

#include <sstream>

FormattedTable::FormattedTable(std::vector<std::string> headers)
{
    this->headers = headers;
}

void FormattedTable::addRow(const std::vector<std::string>& row)
{
    if (row.size() != this->headers.size())
        throw std::runtime_error("Row size does not match header size");
    this->rows.push_back(row);
}

void FormattedTable::computeColumnWidths() const
{
    this->col_widths.resize(this->headers.size(), 0);

    // header lengths
    for (size_t i = 0; i < this->headers.size(); ++i)
        this->col_widths[i] = this->headers[i].size();

    // check rows
    for (const auto& row : this->rows)
        for (size_t i = 0; i < row.size(); ++i)
            this->col_widths[i] = std::max(this->col_widths[i], row[i].size());

    // add one space padding on each side
    for (auto& w : this->col_widths)
        w += 2;
}

std::string FormattedTable::centerText(const std::string& text, size_t width) const
{
    size_t total_padding = width > text.size() ? width - text.size() : 0;
    size_t pad_left = total_padding / 2;
    size_t pad_right = total_padding - pad_left;
    return std::string(pad_left, ' ') + text + std::string(pad_right, ' ');
}

std::string FormattedTable::formatRow(const Row& row) const
{
    std::ostringstream oss;
    for (size_t i = 0; i < row.size(); ++i)
    {
        if (i != 0)
            oss << "|";
        oss << centerText(row[i], this->col_widths[i]);
    }
    return oss.str();
}

std::string FormattedTable::separatorLine() const
{
    std::ostringstream oss;
    for (size_t i = 0; i < this->col_widths.size(); ++i)
    {
        if (i != 0)
            oss << "+";
        oss << std::string(this->col_widths[i], '-');
    }
    return oss.str();
}

std::string FormattedTable::format(int highlightRow) const
{
    if (this->headers.empty())
        return "";

    this->computeColumnWidths();
    std::ostringstream oss;

    const std::string indent = "   "; // default indent
    const std::string arrow = "->"; // highlighted row

    // header
    oss << indent << this->formatRow(this->headers) << "\n";
    oss << indent << this->separatorLine() << "\n";

    // rows
    for (size_t i = 0; i < this->rows.size(); ++i)
    {
        if (static_cast<int>(i) == highlightRow)
            oss << " " << arrow << this->formatRow(this->rows[i]) << "\n";
        else
            oss << indent << formatRow(this->rows[i]) << "\n";
    }

    return oss.str();
}
