# hardware/pcb

**PCB layout** data for the ESP32-EASY-PROTO-V2: source files, Gerbers, BOM, and assembly instructions. **Proprietary**—see [../../LICENSE.md](../../LICENSE.md).

## Directory Structure

- **source/**  
  - Native PCB layout files (e.g., `.kicad_pcb`, `.brd`, `.pcbdoc`).  
- **gerbers/**  
  - Final Gerber files for PCB fabrication.  
  - Drill files, assembly drawings, pick-and-place data (if applicable).  
- **BOM/**  
  - CSV or Excel listing all components, references, and part numbers.  
- **README.md**  
  - (This file) Describes PCBA instructions and revision notes.

## PCB Information

- **Dimensions:** 136.7 mm x 80 mm  
- **Layers:** 2-layer FR-4  
- **Surface Finish:** HASL (with lead)  
- **Solder Mask:** Black, Silkscreen: White  
- **Mounting Holes:** 4 corner holes for rubber feet or enclosure standoffs  

## Revision History

- **V2.0**  
  - Initial production design: Silkscreen had the GPIO36 ↔ GPIO39 swap.  

## Manufacturing Notes

1. **Gerbers**  
   - Provide the entire gerber folder to your PCB manufacturer.  
2. **BOM**  
   - Use the included BOM in `BOM/` subfolder to source components.  
3. **Assembly**  
   - If you plan to have an assembly house place parts, generate the pick-and-place files.

