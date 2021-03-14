import requests
import vlc
from time import sleep

urls = [
        ['http://stream.srg-ssr.ch/drs1/mp3_128.m3u', "srf1" ],
        ['http://stream.srg-ssr.ch/drs2/mp3_128.m3u', "srf2"],
        ['http://stream.srg-ssr.ch/drs3/mp3_128.m3u', "srf3"],
        ['https://energybern.ice.infomaniak.ch/energybern-high.mp3', "enery-bern" ],
        ['https://energybasel.ice.infomaniak.ch/energybasel-high.mp3', "enery-basel" ],
        ['http://radiofribourg.ice.infomaniak.ch/radiofribourg-high.mp3', "Radio fr FR" ],
        ['http://radiofribourg.ice.infomaniak.ch/radiofreiburg-high.mp3', "Radio fr DE" ]
        
        
        
        ]
Instance = vlc.Instance()
player = Instance.media_player_new()
list_player = Instance.media_list_player_new()

def main():
    station = -1
    while True:
        choose = input("\n")
        station = station + 1
        next((station) % len(urls))

    print("setup done")

def next(station):
    global list_player
    list_player.pause()
    Instance = vlc.Instance()
    player = Instance.media_player_new()
    list_player = Instance.media_list_player_new()
    
    url = urls[station][0]
    print("playing station : " + urls[station][1] + "\n")
    if(requests.get(url, stream=True).ok == False):
        print("URL Not Working")
        return
    print(url)
    Media = Instance.media_new(url)
    Media_list = Instance.media_list_new([url])
    Media.get_mrl()
    player.set_media(Media)
    list_player.set_media_list(Media_list)
    list_player.play()
    sleep(3)

if __name__ == "__main__":
    main()