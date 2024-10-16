# MES_Software_SemaphoreTask

# Hardware
CY8KIT-5LP Set

# Goal
Use semaphores to synchronize across tasks.

# Function Description
Adapt the Project Template (with the LED Task included) so a semaphore is freed when the button is pressed. If the semaphore is free the red LED is lit and the duty cycles of the green and blue LED changes. The green LED starts at 0 (off) and increments each 20ms by 1, up to 255 (fully lit). If 255 is reached, jump to 0 and repeat. The blue LED changes the opposite way, start at 255 and decrements every 20ms by 1 until 0 is reached, then jump to 255 and repeats. When the button is released, the green and blue LEDs remain lit with their current duty cycles, and the red LED turns off. Each LED and the button shall be controlled by a dedicated task, all LED tasks shell pend on the same semaphore.

# Schematic Illustration
The switch S1 must be set to “BTN”

# Points
9/10
