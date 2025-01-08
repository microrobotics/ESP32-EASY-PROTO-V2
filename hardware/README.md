# hardware

All **hardware design files** for the **ESP32-EASY-PROTO-V2** board. This includes schematics, PCB layouts, Gerbers, mechanical references, and BOMs. **Proprietary** design—see [../LICENSE.md](../LICENSE.md).

## Subdirectories

- **schematic/**  
  Source schematic files (e.g., KiCad, Altium, Eagle), exported PDF versions, netlists and notes.

- **pcb/**  
  PCB layout files, Gerbers, Bill of Materials (BOM), and related manufacturing outputs.

- **mechanical/**  
  3D models, enclosure drawings, or any mechanical references (STEP/IGES/SLDPRT files).

## Usage

1. **schematic/**  
   - Open the native schematic file in your preferred ECAD tool.  
   - PDF exports are available for quick reference or review.

2. **pcb/**  
   - Contains the board layout source files and final Gerbers for manufacturing.  
   - BOM subfolder (or CSV/Excel file) includes part numbers, references, and sources.

3. **mechanical/**  
   - 3D representations (if any) of the assembled board or mounting designs.  
   - Great for enclosure prototyping and verifying mounting hole positions.

## Notes

- The board is constructed on **FR-4**, 2-layer, black solder mask, HASL finish.  
- Dimensions: 136.7 mm x 90 mm, with corner mounting holes.  
- See `pcb/README.md` and `schematic/README.md` for revision histories and detailed instructions.
