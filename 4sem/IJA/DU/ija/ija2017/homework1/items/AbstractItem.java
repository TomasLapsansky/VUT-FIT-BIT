package ija.ija2017.homework1.items;

import ija.ija2017.repository.Item;

public abstract class AbstractItem implements Item {

    private String name;
    private int port;
    protected int state;

    public AbstractItem(String name, int port) {
        this.name = name;
        this.port = port;
        this.state = 0;
    }

    @Override
    public String getName() {
        return this.name;
    }

    @Override
    public int numberOfPorts() {
        return this.port;
    }

    public abstract void execute();

    @Override
    public int getState() {
        return this.state;
    }

    @Override
    public boolean equals(Object obj) {

        if(!(obj instanceof AbstractItem))
            return false;

        if(((AbstractItem) obj).numberOfPorts() != this.port)
            return false;

        if(!(((AbstractItem) obj).getName().equals(this.name)))
            return false;

        return true;

    }

    @Override
    public int hashCode() {
        return (this.port * this.name.length());
    }
}
