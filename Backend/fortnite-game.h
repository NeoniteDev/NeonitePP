#pragma once
#include "../Frontend/util.h"

json rFortniteGame =
R"(
{
            "jcr:isCheckedOut": true,
            "_title": "Fortnite Game",
            "jcr:baseVersion": "a7ca237317f1e7b00bc82e-d9a2-4f0d-b951-704d295cd1aa",
            "_activeDate": "2017-07-24T22:24:02.300Z",
            "lastModified": "2020-11-01T17:36:19.024Z",
            "_locale": "en-US",
            "emergencynotice": {
                "news": {
                    "platform_messages": [],
                    "_type": "Battle Royale News",
                    "messages": [{
                        "hidden": false,
                        "_type": "CommonUI Simple Message Base",
                        "subgame": "br",
                        "body": "Made by kemo (@xkem0x).\r\nDiscord: https://discord.gg/DJ6VUmD",
                        "title": "Neonite++",
                        "spotlight": false
                    }
                    ]
                },
                "jcr:isCheckedOut": true,
                "_title": "emergencynotice",
                "_noIndex": false,
                "alwaysShow": false,
                "jcr:baseVersion": "a7ca237317f1e761d4ee60-7c40-45a8-aa3e-bb0a2ffa9bb5",
                "_activeDate": "2018-08-06T19:00:26.217Z",
                "lastModified": "2020-10-30T04:50:59.198Z",
                "_locale": "en-US"
            },
            "dynamicbackgrounds": {
                "jcr:isCheckedOut": true,
                "backgrounds": {
                    "backgrounds": [{
                        "stage": "season14",
                        "_type": "DynamicBackground",
                        "key": "lobby"
                    }, {
                        "stage": "season14",
                        "_type": "DynamicBackground",
                        "key": "vault"
                    }],
                    "_type": "DynamicBackgroundList"
                },
                "_title": "dynamicbackgrounds",
                "_noIndex": false,
                "jcr:baseVersion": "a7ca237317f1e7bac3355e-3ec1-4054-baf5-f88613799961",
                "_activeDate": "2019-08-21T15:59:59.342Z",
                "lastModified": "2020-08-27T08:04:47.051Z",
                "_locale": "en-US"
            },
            "shopSections": {
                "jcr:isCheckedOut": true,
                "_title": "shop-sections",
                "sectionList": {
                    "_type": "ShopSectionList",
                    "sections": []
                },
                "_noIndex": false,
                "jcr:baseVersion": "a7ca237317f1e77f7abaac-2b4d-4df8-828b-6056a1904470",
                "_activeDate": "2020-10-28T00:00:00.000Z",
                "lastModified": "2020-11-02T15:56:44.648Z",
                "_locale": "en-US"
            },
            "battleroyalenewsv2": {
                "news": {
                    "motds": [{
                        "entryType": "Website",
                        "image": "https://cdn.discordapp.com/attachments/703974059121049720/715300713995042877/Fornite1920x1080.jpg",
                        "tileImage": "https://cdn.discordapp.com/attachments/703974059121049720/715300717837156362/FortniteMidLogo.jpg",
                        "videoMute": false,
                        "hidden": false,
                        "tabTitleOverride": "Neonite++",
                        "_type": "CommonUI Simple Message MOTD",
                        "title": "Neonite++",
                        "body": "Made by Kemo (@xkem0x). If you have any bugs, you can join our Discord by clicking the button below.",
                        "videoLoop": false,
                        "videoStreamingEnabled": false,
                        "sortingPriority": 80,
                        "id": "NeoniteNewsBR",
                        "videoAutoplay": false,
                        "videoFullscreen": false,
                        "spotlight": false,
                        "websiteURL": "https://discord.gg/DJ6VUmD",
                        "websiteButtonTex" : "Join our discord"
                    }
                ]
                },
                "jcr:isCheckedOut": true,
                "_title": "battleroyalenewsv2",
                "header": "",
                "style": "None",
                "_noIndex": false,
                "alwaysShow": false,
                "jcr:baseVersion": "a7ca237317f1e704b1a186-6846-4eaa-a542-c2c8ca7e7f29",
                "_activeDate": "2020-01-21T14:00:00.000Z",
                "lastModified": "2020-11-01T17:36:19.024Z",
                "_locale": "en-US"
            },
            "shopCarousel": {
                "jcr:isCheckedOut": true,
                "itemsList": {
                    "_type": "ShopCarouselItemList",
                    "items": [{
                        "tileImage": "https://media.discordapp.net/attachments/703974059121049720/774007298786983985/NeoniteWallpaper1920x1080.png",
                        "fullTitle": "Neonite",
                        "hidden": false,
                        "_type": "ShopCarouselItem",
                        "landingPriority": 100,
                        "action": "ShowOfferDetails",
                        "offerId": "v2:/cc29bec6d0bdd2876fb412cf674a7960003bbb76b2eb9b930833a61b604a3b4e",
                        "title": "Neonite"
                    }]
                },
                "_title": "shop-carousel",
                "_noIndex": false,
                "jcr:baseVersion": "a7ca237317f1e76d4c1405-e84c-4ece-bc50-7112ba557b33",
                "_activeDate": "2020-09-25T12:00:00.000Z",
                "lastModified": "2020-10-26T15:37:05.775Z",
                "_locale": "en-US"
            },
            "_suggestedPrefetch": []
        }
)"_json;

inline json fortniteGame()
{
	return rFortniteGame;
}
