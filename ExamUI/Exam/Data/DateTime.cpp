#include "DateTime.h"
#include<iostream>

DateTime::DateTime() :year(1800), month(1), day(1), hour(0), min(0), sec(0) {}

DateTime::DateTime( unsigned int _year, unsigned int _month, unsigned int _day,
                    unsigned int _hour, unsigned int _min, unsigned int _sec)
                    : year(_year), month(_month), day(_day),
                      hour(_hour), min(_min), sec(_sec)
{
    normalizeTime();
}

std::string DateTime::getDate() const
{
    std::string result = "";
    if (day<10) result.append("0");
    result.append(std::to_string(day) + ".");
    if (month < 10) result.append("0");
    result.append(std::to_string(month) + "." + std::to_string(year));
    return result;
};

std::string DateTime::getTime() const { 
    std::string result = "";
    if (hour < 10) result.append("0");
    result.append(std::to_string(hour) + ":");
    if (min < 10) result.append("0");
    result.append(std::to_string(min) + ":");
    if (sec < 10) result.append("0");
    result.append(std::to_string(sec));
    return result; return result;
}

std::string DateTime::getDateTime() const { return getDate() + " " + getTime(); }

bool DateTime::operator==(const DateTime& date2)const {
    if (year != date2.getYear())
        return false;
    if (month != date2.getMonth())
        return false;
    if (day != date2.getDay())
        return false;
    if (hour != date2.getHour())
        return false;
    if (min != date2.getMin())
        return false;
    if (sec != date2.getSec())
        return false;
    return true;
}

bool DateTime::operator!=(const DateTime& date2)const {
    return !(*this == date2);
}

bool DateTime::operator<(const DateTime& date2)const {
    if (year > date2.getYear())
        return false; 
    if (year == date2.getYear()) {
        if (month > date2.getMonth())
            return false;
        if (month == date2.getMonth()) {
            if (day > date2.getDay())
                return false;
            if (day == date2.getDay()) {
                if (hour > date2.getHour())
                    return false;
                if (hour == date2.getHour()) {
                    if (min > date2.getMin())
                        return false;
                    if (min == date2.getMin()) {
                        if (sec >= date2.getSec())
                            return false;
                    }
                }
            }     
        }
    }
    
    return true;
}

bool DateTime::operator<=(const DateTime& date2)const {
    return (*this < date2 || *this == date2);
}

bool DateTime::operator>(const DateTime& date2)const {
    return !(*this <= date2);
}

bool DateTime::operator>=(const DateTime& date2)const {
    return !(*this < date2);
}

void DateTime::normalizeTime()
{
    while (month > 12) {
        year++;
        month -= 12;
    }
    if (sec >= 60) {
        min += sec / 60;
        sec %= 60;
    }
    if (min >= 60) {
        hour += min / 60;
        min %= 60;
    }
    if (hour >= 24) {
        addDays(hour / 24);
        hour %= 24;
    }
    
}

inline bool DateTime::isLeapYear(int _year) const
{
    return _year % 4 == 0 && (_year % 100 != 0 || _year % 400 == 0);
}

void DateTime::addDays(int count_of_days)
{
    day += count_of_days;
    int nMaxDays(month_array[month] + (month == 2 && isLeapYear(year) ? 1 : 0));

    while (day > nMaxDays) {
        day -= nMaxDays;
        ++month;
        if (month > 12) {
            month = 1; 
            ++year;   
        }
        nMaxDays = month_array[month] + (month == 2 && isLeapYear(year) ? 1 : 0);
    }
}