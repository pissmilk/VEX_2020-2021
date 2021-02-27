/* MIT License
 *
 * Copyright (c) 2021 Brighton Sikarskie
 *  
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "math.h"
#include "robotconfig.h"
#include "robotstate.h"

using namespace vex;

const float slowDown = 0.8;

void autonomous();
void usercontrol();

int main() {
    Competition.autonomous(autonomous);
    Competition.drivercontrol(usercontrol);    
}

void autonomous() {
    inert.calibrate();
    task::sleep(2000);
}

void usercontrol() {
    for (;;) {
        rfdrive.spin(directionType::fwd, (Controller1.Axis3.value() - (slowDown * Controller1.Axis1.value())), velocityUnits::pct);
        rbdrive.spin(directionType::fwd, (Controller1.Axis3.value() - (slowDown * Controller1.Axis1.value())), velocityUnits::pct);
        lfdrive.spin(directionType::fwd, (Controller1.Axis3.value() + (slowDown * Controller1.Axis1.value())), velocityUnits::pct);
        lbdrive.spin(directionType::fwd, (Controller1.Axis3.value() + (slowDown * Controller1.Axis1.value())), velocityUnits::pct);
        opti.setLight(ledState::on);
        opti.setLightPower(100, pct);
        
        if (Controller1.ButtonL2.pressing()) {
            roller.setVelocity(100, percent);
            elevator.setVelocity(80, percent);
            if (opti.hue() > 50) {
                elevator.spin(fwd);
                roller.spin(reverse);
            } else {
                elevator.spin(forward);
                roller.spin(forward);
            }
            elevator.spin(forward);
            roller.spin(forward);
        } else if (Controller1.ButtonL1.pressing()) {
            elevator.spin(directionType::rev, 200, velocityUnits::pct);
            roller.spin(directionType::rev, 200, velocityUnits::pct);
        } else {
            roller.stop(brakeType::brake);
            elevator.stop(brakeType::brake);
        }
          
        if (Controller1.ButtonR1.pressing()) {
            lintake.spin(directionType::rev, 100, velocityUnits::pct);
            rintake.spin(directionType::rev, 100, velocityUnits::pct);
        } else if (Controller1.ButtonR2.pressing()) {
            lintake.spin(directionType::fwd, 100, velocityUnits::pct);
            rintake.spin(directionType::fwd, 100, velocityUnits::pct);
        } else {
            lintake.stop(brakeType::brake);
            rintake.stop(brakeType::brake);
        }
        wait(15, msec);
    }
}
