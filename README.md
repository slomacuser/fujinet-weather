# fujinet-weather
The canonical weather application for Atari, ADAM, C64, Coco, and Apple2

June 2024 - This is now in it's own repo. It has been pulled from fujinet-apps and moved here.
Please make all forks and enhancements to this repo via fork and PR.

-----------------------


## Building

### Atari
- need the Mad-Pascal (MP) 32-bit Turbo Pascal compiler - https://mads.atari8.info/doc/en/index.html
- no more info currently

### ADAM
- need zcc compiler for ADAM.
  This version has the following features
  1) Option feature the change polling frequency
  2) Only asks for weather no more than once every 10 minutes
  3) Calls location by ip only once instead of everytime the application is run
  4) You can manually enter the latitude and logitude if desired

### Apple2
- cc65 & fn-libs (pulled automatically)

### c64
- cc65

### Coco
- need cmoc compiler -- more info needed

### VIC-20
- cc65


## Usage

### Apple 2
- Apple2 builds fine but it requires a API key from OpenWeatherMap
  - https://home.openweathermap.org
  - You need to setup an account, request on-by-one and get an API Key
  - You then must upgrade that key to be "Base Plan" - which is free for first 1000 calls
    - you will need to register your name a credit card to get the plan

 This isn't optimal but it's what required right now in June 2024, other API plans will be explored.


 ## Other Platforms

 Work as expected..

