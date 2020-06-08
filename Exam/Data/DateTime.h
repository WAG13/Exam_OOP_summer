#pragma once
#include <string>

class DateTime
{
public:
	DateTime();
	DateTime(   unsigned int year,
                unsigned int month,
                unsigned int day,
                unsigned int hour,
                unsigned int min,
                unsigned int sec);
    ~DateTime() {};

    // Getters
    std::string getDate()     const;
    std::string getTime()     const;
    std::string getDateTime() const;
    unsigned int getYear()    const { return year; }
    unsigned int getMonth()   const { return month; }
    unsigned int getDay()     const { return day; }
    unsigned int getHour()    const { return hour; }
    unsigned int getMin()     const { return min; }
    unsigned int getSec()     const { return sec; }

    // Setters
    void setYear(unsigned int _year) { year = _year; }
    void getMonth(unsigned int _month) { month = _month; }
    void setDay(unsigned int _day)   { day = _day; }
    void setHour(unsigned int _hour) { hour = _hour; }
    void setMin(unsigned int _min)   { min = _min; }
    void setSec(unsigned int _sec)   { sec = _sec; }

    // adds count_of_days days to current date
    void addDays(int count_of_days);

    // Operators
    bool operator==(const DateTime& date2)  const;
    bool operator!=(const DateTime& date2)  const;
    bool operator<(const DateTime& date2)   const;
    bool operator<=(const DateTime& date2)  const;
    bool operator>(const DateTime& date2)   const;
    bool operator>=(const DateTime& date2)  const;

private:
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int min;
    unsigned int sec;
    int month_array[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
protected:
    inline bool isLeapYear(int _year) const;
    void normalizeTime();
};