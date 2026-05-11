#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <memory>
#include <limits>

using namespace std;

// Base class for all employees
class Employee {
protected:
    string name;
    int employeeId;
    double baseSalary;
    double healthInsurance;
    double retirement401k;
    
public:
    Employee(string n, int id, double salary) 
        : name(n), employeeId(id), baseSalary(salary), 
          healthInsurance(200.0), retirement401k(0.0) {}
    
    virtual ~Employee() {}
    
    // Pure virtual functions - must be implemented by derived classes
    virtual double calculateGrossPay() = 0;
    virtual double calculateOvertime() = 0;
    virtual string getEmployeeType() = 0;
    
    // Calculate tax based on progressive tax brackets
    double calculateTax(double grossPay) {
        double tax = 0.0;
        
        if (grossPay <= 10000) {
            tax = grossPay * 0.10;  // 10% for income up to $10,000
        } else if (grossPay <= 40000) {
            tax = 1000 + (grossPay - 10000) * 0.15;  // 15% for $10,001 - $40,000
        } else if (grossPay <= 85000) {
            tax = 5500 + (grossPay - 40000) * 0.22;  // 22% for $40,001 - $85,000
        } else {
            tax = 15400 + (grossPay - 85000) * 0.28;  // 28% for $85,001+
        }
        
        return tax;
    }
    
    // Calculate total benefits deductions
    double calculateBenefitsDeduction() {
        retirement401k = calculateGrossPay() * 0.06;  // 6% retirement contribution
        return healthInsurance + retirement401k;
    }
    
    // Calculate net pay
    double calculateNetPay() {
        double grossPay = calculateGrossPay();
        double tax = calculateTax(grossPay);
        double benefits = calculateBenefitsDeduction();
        return grossPay - tax - benefits;
    }
    
    // Display payroll details
    virtual void displayPayrollDetails() {
        double grossPay = calculateGrossPay();
        double overtime = calculateOvertime();
        double tax = calculateTax(grossPay);
        double benefits = calculateBenefitsDeduction();
        double netPay = calculateNetPay();
        
        cout << "\n" << string(60, '=') << endl;
        cout << "PAYROLL STATEMENT" << endl;
        cout << string(60, '=') << endl;
        cout << left << setw(25) << "Employee Name:" << name << endl;
        cout << left << setw(25) << "Employee ID:" << employeeId << endl;
        cout << left << setw(25) << "Employee Type:" << getEmployeeType() << endl;
        cout << string(60, '-') << endl;
        
        cout << fixed << setprecision(2);
        cout << left << setw(25) << "Base Salary:" << "$" << baseSalary << endl;
        cout << left << setw(25) << "Overtime Pay:" << "$" << overtime << endl;
        cout << left << setw(25) << "Gross Pay:" << "$" << grossPay << endl;
        cout << string(60, '-') << endl;
        
        cout << "DEDUCTIONS:" << endl;
        cout << left << setw(25) << "  Federal Tax:" << "$" << tax << endl;
        cout << left << setw(25) << "  Health Insurance:" << "$" << healthInsurance << endl;
        cout << left << setw(25) << "  401(k) Contribution:" << "$" << retirement401k << endl;
        cout << left << setw(25) << "  Total Deductions:" << "$" << (tax + benefits) << endl;
        cout << string(60, '-') << endl;
        
        cout << left << setw(25) << "NET PAY:" << "$" << netPay << endl;
        cout << string(60, '=') << endl;
    }
    
    // Getters
    string getName() const { return name; }
    int getEmployeeId() const { return employeeId; }
};

// Full-time employee class
class FullTimeEmployee : public Employee {
private:
    double annualSalary;
    double overtimeHours;
    double overtimeRate;
    
public:
    FullTimeEmployee(string n, int id, double salary, double otHours = 0.0) 
        : Employee(n, id, salary), annualSalary(salary), 
          overtimeHours(otHours), overtimeRate(1.5) {}
    
    double calculateGrossPay() override {
        double monthlySalary = annualSalary / 12.0;
        double hourlyRate = annualSalary / (52.0 * 40.0);  // Assuming 40-hour work week
        double overtimePay = overtimeHours * hourlyRate * overtimeRate;
        return monthlySalary + overtimePay;
    }
    
    double calculateOvertime() override {
        double hourlyRate = annualSalary / (52.0 * 40.0);
        return overtimeHours * hourlyRate * overtimeRate;
    }
    
    string getEmployeeType() override {
        return "Full-Time Employee";
    }
    
    void setOvertimeHours(double hours) {
        overtimeHours = hours;
    }
};

// Part-time employee class
class PartTimeEmployee : public Employee {
private:
    double hourlyRate;
    double hoursWorked;
    
public:
    PartTimeEmployee(string n, int id, double rate, double hours) 
        : Employee(n, id, rate * hours), hourlyRate(rate), hoursWorked(hours) {
        healthInsurance = 100.0;  // Reduced benefits for part-time
    }
    
    double calculateGrossPay() override {
        double regularPay = 0.0;
        double overtimePay = 0.0;
        
        if (hoursWorked <= 160) {  // Up to 160 hours per month (no overtime)
            regularPay = hoursWorked * hourlyRate;
        } else {  // Overtime for hours > 160
            regularPay = 160 * hourlyRate;
            overtimePay = (hoursWorked - 160) * hourlyRate * 1.5;
        }
        
        return regularPay + overtimePay;
    }
    
    double calculateOvertime() override {
        if (hoursWorked > 160) {
            return (hoursWorked - 160) * hourlyRate * 1.5;
        }
        return 0.0;
    }
    
    string getEmployeeType() override {
        return "Part-Time Employee";
    }
    
    void setHoursWorked(double hours) {
        hoursWorked = hours;
    }
};

// Contractor class (independent contractor)
class Contractor : public Employee {
private:
    double contractRate;
    double hoursWorked;
    
public:
    Contractor(string n, int id, double rate, double hours) 
        : Employee(n, id, rate * hours), contractRate(rate), hoursWorked(hours) {
        healthInsurance = 0.0;  // No benefits for contractors
    }
    
    double calculateGrossPay() override {
        return contractRate * hoursWorked;
    }
    
    double calculateOvertime() override {
        return 0.0;  // Contractors don't get overtime
    }
    
    string getEmployeeType() override {
        return "Contractor";
    }
    
    // Override benefits calculation - contractors don't get 401k
    double calculateBenefitsDeduction() {
        return 0.0;
    }
    
    // Override tax calculation - contractors pay self-employment tax
    double calculateTax(double grossPay) {
        double incomeTax = Employee::calculateTax(grossPay);
        double selfEmploymentTax = grossPay * 0.153;  // 15.3% self-employment tax
        return incomeTax + selfEmploymentTax;
    }
    
    void setHoursWorked(double hours) {
        hoursWorked = hours;
    }
};

// Payroll Manager class to manage multiple employees
class PayrollManager {
private:
    vector<shared_ptr<Employee>> employees;
    
public:
    void addEmployee(shared_ptr<Employee> emp) {
        employees.push_back(emp);
    }
    
    void processPayroll() {
        if (employees.empty()) {
            cout << "\nNo employees to process!" << endl;
            return;
        }
        
        cout << "\n\n" << string(60, '*') << endl;
        cout << "PROCESSING COMPANY PAYROLL" << endl;
        cout << string(60, '*') << endl;
        
        double totalGross = 0.0;
        double totalNet = 0.0;
        
        for (auto& emp : employees) {
            emp->displayPayrollDetails();
            totalGross += emp->calculateGrossPay();
            totalNet += emp->calculateNetPay();
        }
        
        cout << "\n" << string(60, '*') << endl;
        cout << "PAYROLL SUMMARY" << endl;
        cout << string(60, '*') << endl;
        cout << fixed << setprecision(2);
        cout << "Total Employees: " << employees.size() << endl;
        cout << "Total Gross Payroll: $" << totalGross << endl;
        cout << "Total Net Payroll: $" << totalNet << endl;
        cout << "Total Deductions: $" << (totalGross - totalNet) << endl;
        cout << string(60, '*') << endl;
    }
    
    void displayEmployeeList() {
        if (employees.empty()) {
            cout << "\nNo employees registered yet!" << endl;
            return;
        }
        
        cout << "\n\nEMPLOYEE ROSTER:" << endl;
        cout << string(60, '-') << endl;
        for (const auto& emp : employees) {
            cout << "ID: " << emp->getEmployeeId() 
                 << " | " << emp->getName() 
                 << " | " << emp->getEmployeeType() << endl;
        }
        cout << string(60, '-') << endl;
    }
};

// Helper function to clear input buffer
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper function to get valid integer input
int getIntInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            clearInputBuffer();
            return value;
        } else {
            cout << "Invalid input! Please enter a number." << endl;
            clearInputBuffer();
        }
    }
}

// Helper function to get valid double input
double getDoubleInput(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0) {
            clearInputBuffer();
            return value;
        } else {
            cout << "Invalid input! Please enter a positive number." << endl;
            clearInputBuffer();
        }
    }
}

// Helper function to get string input
string getStringInput(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

int main() {
    cout << "\n*** EMPLOYEE PAYROLL SYSTEM ***\n" << endl;
    cout << "Interactive Mode - Enter Employee Details at Runtime\n" << endl;
    
    PayrollManager payroll;
    bool continueAdding = true;
    
    while (continueAdding) {
        cout << "\n" << string(60, '=') << endl;
        cout << "ADD NEW EMPLOYEE" << endl;
        cout << string(60, '=') << endl;
        
        // Get employee type
        cout << "\nSelect Employee Type:" << endl;
        cout << "1. Full-Time Employee (Salaried)" << endl;
        cout << "2. Part-Time Employee (Hourly)" << endl;
        cout << "3. Contractor (Independent)" << endl;
        
        int empType = getIntInput("Enter choice (1-3): ");
        
        if (empType < 1 || empType > 3) {
            cout << "Invalid choice! Skipping this employee." << endl;
            continue;
        }
        
        // Get common details
        string name = getStringInput("Enter employee name: ");
        int id = getIntInput("Enter employee ID: ");
        
        shared_ptr<Employee> newEmployee;
        
        switch (empType) {
            case 1: {
                // Full-Time Employee
                double annualSalary = getDoubleInput("Enter annual salary ($): ");
                double overtimeHours = getDoubleInput("Enter overtime hours this month: ");
                newEmployee = make_shared<FullTimeEmployee>(name, id, annualSalary, overtimeHours);
                cout << "\nFull-time employee added successfully!" << endl;
                break;
            }
            case 2: {
                // Part-Time Employee
                double hourlyRate = getDoubleInput("Enter hourly rate ($): ");
                double hoursWorked = getDoubleInput("Enter hours worked this month: ");
                newEmployee = make_shared<PartTimeEmployee>(name, id, hourlyRate, hoursWorked);
                cout << "\nPart-time employee added successfully!" << endl;
                break;
            }
            case 3: {
                // Contractor
                double contractRate = getDoubleInput("Enter contract hourly rate ($): ");
                double hoursWorked = getDoubleInput("Enter hours worked this month: ");
                newEmployee = make_shared<Contractor>(name, id, contractRate, hoursWorked);
                cout << "\nContractor added successfully!" << endl;
                break;
            }
        }
        
        payroll.addEmployee(newEmployee);
        
        // Ask if user wants to add more employees
        cout << "\nDo you want to add another employee? (1=Yes, 0=No): ";
        int choice;
        cin >> choice;
        continueAdding = (choice == 1);
    }
    
    // Display employee roster
    payroll.displayEmployeeList();
    
    // Process payroll for all employees
    payroll.processPayroll();
    
    cout << "\n\n*** PAYROLL PROCESSING COMPLETE ***\n" << endl;
    
    return 0;
}
