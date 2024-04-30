## Q1
To use Bluetooth technology as a replacement for RFID in an authentication system for a door lock, you would typically need the following components:

    Bluetooth-enabled Door Lock: This is the device that controls access to the door. It contains the locking mechanism and is equipped with Bluetooth connectivity capabilities to communicate with authorized devices.

    Mobile Devices: Instead of RFID tags, users would use their smartphones or other Bluetooth-enabled devices as their means of authentication. These devices would need to have a compatible app installed for authentication purposes.

    
## Q2
to transfer necessary information, such as a person's ID, between Bluetooth modules without requiring them to be paired. This can be achieved using Bluetooth Low Energy (BLE) technology and a broadcasting method called Bluetooth advertising.

    
    1. Advertising Mode: The Bluetooth module (e.g., the door acting as a base station) operates in advertising mode. It periodically broadcasts advertising packets containing relevant information, such as the door's identity and services offered.

    2. Scanning Mode: Bluetooth-enabled devices (e.g., smartphones carried by individuals) operate in scanning mode. They continuously scan for nearby Bluetooth devices and listen for advertising packets.

    3. Data Payload: The advertising packets broadcast by the door can include a custom data payload that contains the necessary information for identification, such as the person's ID.

    4. Data Extraction: When a smartphone detects an advertising packet from the door, it extracts the custom data payload from the packet. This payload contains the person's ID or other relevant information.

    5. Authentication: The smartphone app processes the extracted information, such as the person's ID, and initiates the authentication process with the door. This could involve sending the person's ID to a server for verification or performing local authentication based on predetermined criteria.

    6. Access Control: If the authentication is successful, the door grants access to the person associated with the provided ID.

## Q3

  1. Bluetooth Beacon at the Entrance:
    Install a Bluetooth beacon at the entrance of the company. This beacon continuously broadcasts Bluetooth signals to detect nearby Bluetooth-enabled devices.

  2. Employee Smartphones as Key Devices:
    Authorize employee smartphones as key devices. Each employee installs a designated app on their smartphone that communicates with the Bluetooth beacon.

  3. Presence Detection:
    When an employee with an authorized smartphone approaches the entrance, the Bluetooth beacon detects the smartphone's presence based on the received signal strength.

  4. Door Open Command:
    Upon detecting an authorized smartphone in proximity, the Bluetooth beacon sends a command to the door control system to keep the entrance open.

  5. Continuous Presence Check:
    The system continuously monitors the presence of authorized smartphones. As long as at least one authorized smartphone remains in the vicinity, the entrance remains open.
    
