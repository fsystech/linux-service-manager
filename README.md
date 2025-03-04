# Linux Service Manager

The **Linux Service Manager** is a C++ application designed to manage and monitor Linux services efficiently. It allows users to start, stop, and monitor services, ensuring they operate within specified time ranges and handle dependencies appropriately.

## Features

- **Service Management**: Start and stop services based on predefined schedules.
- **Dependency Handling**: Manage services with dependencies, ensuring proper startup and shutdown sequences.
- **Time-Based Operations**: Configure services to operate within specific time ranges.
- **Logging**: Maintain logs for service operations and status changes.

## Installation

### Prerequisites

- **C++ Compiler**: Ensure you have a C++17 compatible compiler installed (e.g., `g++`).
- **CMake**: Build system generator for compiling the project.
- **systemd Development Libraries**: Required for interfacing with system services.

### Steps

1. **Clone the Repository**:

   ```bash
   git clone https://github.com/rajibchy/linux-service-manager.git
   cd linux-service-manager
   ```


2. **Create a Build Directory**:

   ```bash
   mkdir build
   cd build
   ```


3. **Generate Build Files**:

   ```bash
   cmake ..
   ```


4. **Compile the Project**:

   ```bash
   make
   ```


5. **Install the Application**:

   ```bash
   sudo make install
   ```


   *Note*: The installation path can be configured by modifying the CMake configuration.

## Usage

After installation, you can use the Linux Service Manager to control and monitor services.

### Starting the Manager


```bash
sudo linux-service-manager
```


### Configuration

The application uses a configuration file to define services, their dependencies, and operational time ranges. By default, it looks for `config.json` in the current directory.

**Sample `config.json`**:


```json
{
  "services": [
    {
      "name": "example.service",
      "dependencies": ["dependency1.service", "dependency2.service"],
      "time_range": {
        "start": "08:00",
        "end": "18:00"
      }
    }
  ]
}
```


- **name**: The name of the service.
- **dependencies**: A list of services that must be started before this service.
- **time_range**: Specifies when the service should be active.

### Logs

Logs are stored in `/var/log/linux-service-manager.log`. Monitor this file to review service operations and statuses.

## Contributing

Contributions are welcome! Please fork the repository and submit a pull request with your enhancements or bug fixes.

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

---
