import time
from selenium import webdriver
from selenium.webdriver.chrome.options import Options
CHROMEDRIVER_PATH = '/usr/local/bin/chromedriver'
WINDOW_SIZE = "1920,1080"
chrome_options = Options()
chrome_options.add_argument("--headless")
chrome_options.add_argument("--window-size=%s" % WINDOW_SIZE)
chrome_options.add_argument('--no-sandbox')
driver = webdriver.Chrome(executable_path=CHROMEDRIVER_PATH,
                          options=chrome_options
                         )
driver.get("http://www.rochesterastronomy.org/snimages/sndate.html")
i = 2
path = "/html/body/table[2]/tbody/tr["+str(i)+"]/td[5]"
str1 = "/dataStorage/supernova.txt"
file = open(str1, "w")


while True:
    element1 = driver.find_element_by_xpath(path)
    year = element1.text.split("/")[0]
    month = element1.text.split("/")[1]
    day = element1.text.split("/")[2]
    nowTime = time.gmtime()
    if nowTime.tm_year != int(year) or nowTime.tm_mon != int(month) or nowTime.tm_mday - 1 > int(day):
        break
    path = "/html/body/table[2]/tbody/tr["+str(i)+"]/td[9]"
    i = i + 1
    element2 = driver.find_element_by_xpath(path)
    file.write(element1.text + " " + element2.text + "\n")
    path = "/html/body/table[2]/tbody/tr["+str(i)+"]/td[5]"

file.close
driver.close()
