
# coding: utf-8

# In[1]:

import time
import json
import tweepy
import wget
CONSUMER_KEY = 'XJ2EaeDDmuUwjblO69X6mXPpA'
CONSUMER_SECRET = '8sersWXewBQh2jBSmNHqyRzRoPysDfiSxTb5WDsnFXhtfBy6fW'
ACCESS_KEY = '721677117273042946-c6dXYM2jKHpRMmI0PMDhhP1MqRLgxGO'
ACCESS_SECRET = 'TV8QwYGYZxAWYgGdhJWf6tTNjE6on0JoPHaCjJ0MihRyf'

media_url=[]
TOTALIMAGES=200
auth = tweepy.OAuthHandler(CONSUMER_KEY, CONSUMER_SECRET)
auth.set_access_token(ACCESS_KEY, ACCESS_SECRET)

api = tweepy.API(auth)

c=tweepy.Cursor(api.search,q="Mice",lang="en") #EDIT var q to search for different tags


    
for result in c.items():
    for media in result.entities.get("media",[{}]):
        if media.get("type",None) == "photo":
            #print "red"
            print media["media_url"]
            #wget.download(media["media_url"])
            media_url.append(media["media_url"])
            print len(media_url)
            # checks if the entity is of the type "photo"
            #image_content=results.get(media["media_url"])
    if(len(media_url)>TOTALIMAGES):
        break
    
        
    

#We've got the media urls now using wget to download the items
for media_file in media_url:
    print media_file
    wget.download(media_file)


# In[ ]:



