package com.example.test_android_kanji;

public class UserInfo
{
    private String id;
    private String name;
    private String email;
    private Boolean unlocked;
    private int hints;

    public UserInfo(String _id, String _name, String _email, int _hints, boolean _unlocked)
    {
        id = _id;
        name = _name;
        email = _email;
        hints = _hints;
        unlocked = _unlocked;
    }

    public UserInfo()
    {
        id = "";
        name = "";
        email = "";
        hints = -1;
        unlocked = false;
    }

    public String getName()
    {
        return name;
    }
    public String getEmail()
    {
        return email;
    }
    public int getHints()
    {
        return hints;
    }
    public String getID()
    {
        return id;
    }
    public Boolean isUnlocked()
    {
        return unlocked;
    }

    public void setName(String _name)
    {
       name = _name;
    }
    public void setID(String _id)
    {
        id = _id;
    }
    public void setEmail(String _email)
    {
        email = _email;
    }
    public void setHints(int _hints)
    {
        hints = _hints;
    }
    public void setUnlock(boolean _unlocked)
    {
        unlocked = _unlocked;
    }
}
