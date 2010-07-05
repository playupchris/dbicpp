require 'do_mysql'
require 'do_postgres'

class Benchmarks

  class DO
    attr_reader :driver
    def initialize driver, sql
      @driver = (driver == :postgresql ? 'postgres' : driver)
      @h   = DataObjects::Connection.new("#{@driver}://127.0.0.1/dbicpp");
      @sth = @h.create_command(sql)
    end

    def run n, *args
      n.times do
        @sth.execute_reader(*args).each {|r| r }
      end
    end

    def self.name
      "DO"
    end
  end

  register :mysql, DO
  register :postgresql, DO
end