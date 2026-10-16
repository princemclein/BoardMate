# BoardMate

**A Boarding House Utility Bill Management System**

BoardMate is a C-based console application designed to help boarding house owners manage rooms, tenant records, and monthly electricity and water bills. It aims to provide a fair and organized way to calculate utility expenses based on tenant occupancy and room usage.

> **Status:** Currently in Development

## Features

### Planned Features

- **Room Management** — Register rooms and define their occupancy capacities.
- **Tenant Management** — Manage tenant information and occupancy periods.
- **Utility Bill Calculation** — Calculate and distribute electricity and water expenses fairly.
- **Person-Day Billing** — Account for the number of days each tenant occupies a room during a billing period.
- **Submeter Support** — Accommodate rooms with individual electricity submeters.
- **Guest-Night Tracking** — Record guest stays for more accurate expense allocation.
- **Brownout Credits** — Account for eligible power interruptions in electricity billing.
- **Electricity Rate Calculation** — Support tiered electricity rates where applicable.
- **Usage Forecasting** — Estimate future utility consumption and expenses.
- **Payment Tracking** — Maintain tenant balances and payment records.
- **Receipt Generation** — Generate text-based billing receipts.
- **File Persistence** — Save records for future use.

*These features are planned and may change as development progresses.*

## Technology Stack

- **Language:** C
- **Compiler:** GCC
- **Environment:** Windows / PowerShell
- **Version Control:** Git and GitHub

## Getting Started

### Prerequisites

- GCC compiler
- Git (optional, for cloning the repository)

### Compile the Program

```bash
gcc boardMate.c -o boardMate
```

### Run the Program

On Windows PowerShell:

```powershell
.\boardMate
```

## Project Structure

```text
BoardMate/
├── boardMate.c
└── README.md
```

Additional source files and folders may be introduced as the project grows.

## Project Goals

BoardMate aims to make boarding-house utility billing more transparent, organized, and fair by accounting for tenant occupancy, room-level usage, and other relevant billing factors.

## Development Note

This project is currently under active development. Features, calculations, and program structure may change as implementation and testing continue.

## License

No license has been specified yet.
