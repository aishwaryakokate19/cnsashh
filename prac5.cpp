#include <iostream>
#include <string>
#include <vector>

using namespace std;

class SubnetCalculator {
public:
    static void main() {
        string ipAddress;

        // Get user input for IP address
        cout << "Enter IP Address: ";
        getline(cin, ipAddress);

        // Calculate default subnet mask and network address
        string subnetMask = getDefaultSubnetMask(ipAddress);
        string networkAddress = calculateNetworkAddress(ipAddress, subnetMask);
        string firstAddress = getFirstAddress(networkAddress, subnetMask);
        string lastAddress = getLastAddress(networkAddress, subnetMask);

        // Output results
        cout << "Subnet Mask: " << subnetMask << endl;
        cout << "Network Address: " << networkAddress << endl;
        cout << "First Address: " << firstAddress << endl;
        cout << "Last Address: " << lastAddress << endl;
    }

private:
    // Determine the default subnet mask based on the IP address class
    static string getDefaultSubnetMask(const string& ipAddress) {
        int firstOctet = stoi(ipAddress.substr(0, ipAddress.find('.')));

        if (firstOctet >= 1 && firstOctet <= 126) return "255.0.0.0";   // Class A
        if (firstOctet >= 128 && firstOctet <= 191) return "255.255.0.0"; // Class B
        if (firstOctet >= 192 && firstOctet <= 223) return "255.255.255.0"; // Class C
        return "0.0.0.0"; // For other classes (D, E) or invalid IP
    }

    // Calculate the network address by applying the subnet mask
    static string calculateNetworkAddress(const string& ipAddress, const string& subnetMask) {
        vector<int> ipOctets = splitToOctets(ipAddress);
        vector<int> maskOctets = splitToOctets(subnetMask);

        // Compute the network address by bitwise AND operation
        vector<int> networkOctets(4);
        for (size_t i = 0; i < 4; ++i) {
            networkOctets[i] = ipOctets[i] & maskOctets[i];
        }

        return joinOctets(networkOctets);
    }

    // Calculate the first address in the subnet (network address + 1)
    static string getFirstAddress(const string& networkAddress, const string& subnetMask) {
        vector<int> networkOctets = splitToOctets(networkAddress);
        vector<int> maskOctets = splitToOctets(subnetMask);

        vector<int> firstAddressOctets = networkOctets;
        for (int i = 3; i >= 0; --i) {
            if (firstAddressOctets[i] < 255) {
                ++firstAddressOctets[i];
                break;
            } else {
                firstAddressOctets[i] = 0;
            }
        }

        return joinOctets(firstAddressOctets);
    }

    // Calculate the last address in the subnet (broadcast address - 1)
    static string getLastAddress(const string& networkAddress, const string& subnetMask) {
        vector<int> networkOctets = splitToOctets(networkAddress);
        vector<int> maskOctets = splitToOctets(subnetMask);

        vector<int> broadcastAddressOctets = networkOctets;
        for (size_t i = 0; i < 4; ++i) {
            broadcastAddressOctets[i] |= ~maskOctets[i] & 255;
        }

        // The last address is the broadcast address minus 1
        vector<int> lastAddressOctets = broadcastAddressOctets;
        for (int i = 3; i >= 0; --i) {
            if (lastAddressOctets[i] > 0) {
                --lastAddressOctets[i];
                break;
            } else {
                lastAddressOctets[i] = 255;
            }
        }

        return joinOctets(lastAddressOctets);
    }

    // Split an IP address or subnet mask into its octets
    static vector<int> splitToOctets(const string& address) {
        vector<int> octets;
        size_t start = 0;
        size_t end = address.find('.');

        // Extract each octet
        while (end != string::npos) {
            octets.push_back(stoi(address.substr(start, end - start)));
            start = end + 1;
            end = address.find('.', start);
        }
        // Add the last octet
        octets.push_back(stoi(address.substr(start)));

        return octets;
    }

    // Join the octets into a string format
    static string joinOctets(const vector<int>& octets) {
        string result;
        for (size_t i = 0; i < octets.size(); ++i) {
            if (i > 0) result += '.';
            result += to_string(octets[i]);
        }
        return result;
    }
};

int main() {
    SubnetCalculator::main();
    return 0;
}
